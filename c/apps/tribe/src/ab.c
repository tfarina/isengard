#include "ab.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "third_party/sqlite/sqlite3.h"

#include "os_path.h"
#include "xstring.h"

/* NOTE: Always increment this when you make incompatible db structure
 * changes */
#define DB_VERSION 0

static sqlite3 *hdb = NULL;  /* SQLite db handle */

static alpm_list_t *contact_list;

static int _db_close(void) {
  int rc;
  int scode = 0; /* success */

  rc = sqlite3_close(hdb);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: Unable to close the database: %s\n",
	    sqlite3_errmsg(hdb));
    scode = -1;
  }
  hdb = NULL;

  return scode;
}

/**
 * Makes sure the 'contacts' table is created if it does not exist yet.
 *
 * @return return 0 on success, -1 otherwise.
 */
static int _db_init_schema(void) {
  int rc;
  char const create_sql[] =
    "CREATE TABLE IF NOT EXISTS contacts ("
    "  id INTEGER PRIMARY KEY,"     /* id */
    "  fname TEXT,"                 /* first name */
    "  lname TEXT,"                 /* last name */
    "  email TEXT"                  /* email */
    ");";
  char *err_msg = NULL;

  rc = sqlite3_exec(hdb, create_sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_exec failed: %s\n", err_msg);
    sqlite3_free(err_msg);
    return -1;
  }

  return rc;
}

static int _db_pragma_integrity_check_cb(void *data, int argc, char **argv, char **column_names) {
  int *errflag = data;

  if (argc == 1) {
    if (0 == strcmp(column_names[0], "integrity_check")) {
      if (0 != strcmp(argv[0], "ok")) {
        /* Error! */
        *errflag = 1;
      }
    }
  }

  return 0;
}

static int _db_pragma_user_version_cb(void *data, int argc, char **argv, char **column) {
  int *id = data;

  /*UNUSED(argc);*/
  /*UNUSED(column);*/

  if (argv[0]) {
    *id = atoi(argv[0]);
  } else {
    *id = 0;
  }

  return 0;
}

int ab_init(char *db_dir) {
  int rc;
  char const db_file_name[] = "abdb.sqlite3";
  char *db_file_path;
  int corrupted = 0;
  int user_version = 0;
  char *err_msg = NULL;

  /* Do nothing if the database handle has been set. */
  if (hdb) {
    return 0;
  }

  db_file_path = os_path_join(db_dir, db_file_name);

  rc = sqlite3_open(db_file_path, &hdb);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: Unable to open the database at %s: %s\n",
	    db_file_path, sqlite3_errmsg(hdb));
    free(db_file_path);
    sqlite3_close(hdb);
    hdb = NULL;
    return -1;
  }
  free(db_file_path);

  rc = sqlite3_exec(hdb, "PRAGMA integrity_check;", _db_pragma_integrity_check_cb, &corrupted, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_exec failed: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(hdb);
    hdb = NULL;
    return -1;
  }

  if (1 == corrupted) {
    fprintf(stderr, "ERROR: The SQLite database integrity check failed. It may be corrupted.\n");
    sqlite3_close(hdb);
    hdb = NULL;
    return -1;
  }

  rc = sqlite3_exec(hdb, "PRAGMA user_version;", _db_pragma_user_version_cb, &user_version, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_exec failed: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(hdb);
    hdb = NULL;
    return -1;
  }

  rc = _db_init_schema();
  if (rc < 0) {
    sqlite3_close(hdb);
    hdb = NULL;
    return -1;
  }

  return 0;
}

int ab_fini(void) {
  int rc;

  FREELIST(contact_list);

  rc = _db_close();
  if (rc < 0) {
    return -1;
  }

  return 0;
}

static int _db_get_contacts_row_count(int *row_count) {
  int rc;
  int scode = 0; /* success */
  char const count_sql[] = "SELECT COUNT(*) FROM contacts";
  sqlite3_stmt *count_stmt = NULL;

  if (!row_count) {
    return -EINVAL;  /* Invalid args */
  }

  rc = sqlite3_prepare_v2(hdb, count_sql, -1, &count_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_prepare_v2 failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_step(count_stmt);
  if (rc != SQLITE_ROW) {
    fprintf(stderr, "ERROR: sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  *row_count = sqlite3_column_int(count_stmt, 0);

out:
  sqlite3_finalize(count_stmt);

  return scode;
}

int _db_enum_contacts(int *num_contacts, ab_contact_t **contacts_dst) {
  int rc;
  int scode = 0;
  int row_count = 0;
  char const select_sql[] = "SELECT * FROM contacts";
  sqlite3_stmt *select_stmt = NULL;
  ab_contact_t *contacts = NULL;
  int i;
  int num_rows = 0;

  if (!num_contacts || !contacts_dst) {
    return -EINVAL;  /* Invalid args */
  }

  rc = _db_get_contacts_row_count(&row_count);
  if (rc < 0) {
    return -1;
  }

  /* Allocate the contacts array
   */
  contacts = malloc(row_count * sizeof(ab_contact_t));
  if (!contacts) {
    scode = -ENOMEM;
    goto err;
  }

  /* Zero init it
   */
  memset(contacts, 0, row_count * sizeof(ab_contact_t));

  rc = sqlite3_prepare_v2(hdb, select_sql, -1, &select_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_prepare_v2 failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto err;
  }

  for (i = 0; i < row_count; i++) {
    char const *psz = NULL;

    rc = sqlite3_step(select_stmt);
    if (rc != SQLITE_ROW)
      break;

    contacts[i].id = sqlite3_column_int(select_stmt, 0);

    psz = (char const *)sqlite3_column_text(select_stmt, 1);
    contacts[i].fname = xstrdup(psz);
    if (!contacts[i].fname) {
      scode = -ENOMEM;
      goto err;
    }

    psz = (char const *)sqlite3_column_text(select_stmt, 2);
    contacts[i].lname = xstrdup(psz);
    if (!contacts[i].lname) {
      scode = -ENOMEM;
      goto err;
    }

    psz = (char const *)sqlite3_column_text(select_stmt, 3);
    contacts[i].email = xstrdup(psz);
    if (!contacts[i].email) {
      scode = -ENOMEM;
      goto err;
    }

    num_rows++;
  }

  *num_contacts = num_rows;
  *contacts_dst = contacts;

err:
  if (scode < 0) {
    free(contacts);
    contacts = NULL;
  }
  sqlite3_finalize(select_stmt);

  return scode;
}

int ab_enum_contacts(alpm_list_t **pp_contact_list) {
  int rc;
  int num_contacts = 0;
  ab_contact_t *contacts = NULL;
  int i;
  ab_contact_t *p_contact = NULL;

  rc = _db_enum_contacts(&num_contacts, &contacts);
  if (rc < 0) {
    goto exit;
  }

  for (i = 0; i < num_contacts; i++) {
    rc = ab_contact_create(&p_contact);
    if (rc < 0 || !p_contact) {
      rc = -1;
      goto exit;
    }

    p_contact->id = contacts[i].id;
    p_contact->fname = xstrdup(contacts[i].fname);
    p_contact->lname = xstrdup(contacts[i].lname);
    p_contact->email = xstrdup(contacts[i].email);

    contact_list = alpm_list_add(contact_list, p_contact);
  }

  *pp_contact_list = contact_list;

exit:
  if (contacts) {
    free(contacts);
    contacts = NULL;
  }

  return rc;
}

int ab_enum_contacts_v2(int *num_contacts, ab_contact_t **contacts_dst) {
  return _db_enum_contacts(num_contacts, contacts_dst);
}

int _db_insert_contact(ab_contact_t *contact) {
  int rc = 0;
  int scode = 0;
  char const insert_sql[] =
      "INSERT INTO contacts (fname, lname, email) VALUES (?, ?, ?)";
  sqlite3_stmt *insert_stmt = NULL;

  rc = sqlite3_prepare_v2(hdb, insert_sql, -1, &insert_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_prepare_v2 failed: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_bind_text(insert_stmt, 1, contact->fname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_text failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(insert_stmt, 2, contact->lname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_text failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(insert_stmt, 3, contact->email, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_text failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_step(insert_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "ERROR: sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
  }

out:
  sqlite3_finalize(insert_stmt);

  return scode;
}

/*
 * Adds the specified contact to the address book database.
 */
int ab_add_contact(ab_contact_t *contact) {
  int rc;

  rc = _db_insert_contact(contact);
  if (rc < 0)
    return -1;

  contact_list = alpm_list_add(contact_list, contact);

  return 0;
}

/*
 * Adds the specified contact to the address book database.
 */
int ab_add_contact_v2(ab_contact_t *contact) {
  return _db_insert_contact(contact);
}

int _db_update_contact(ab_contact_t* contact) {
  int rc;
  int scode = 0;
  char const update_sql[] =
      "UPDATE contacts SET fname=?, lname=?, email=? WHERE id=?";
  sqlite3_stmt *update_stmt = NULL;

  rc = sqlite3_prepare_v2(hdb, update_sql, -1, &update_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_prepare_v2 failed: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_bind_text(update_stmt, 1, contact->fname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_text failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(update_stmt, 2, contact->lname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_text failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(update_stmt, 3, contact->email, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_text failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_bind_int(update_stmt, 4, contact->id);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_int failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_step(update_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "ERROR: sqlite3_step failed: %s\n",
	    sqlite3_errmsg(hdb));
    scode = -1;
  }

out:
  sqlite3_finalize(update_stmt);

  return scode;
}

/*
 * Updates an existing contact in the address book database.
 */
int ab_update_contact(ab_contact_t *contact) {
  return _db_update_contact(contact);
}

static int _db_delete_contact(int id) {
  int rc;
  int scode = 0;
  char const delete_sql[] = "DELETE FROM contacts WHERE id=?";
  sqlite3_stmt *delete_stmt = NULL;
  int num_change = 0;

  rc = sqlite3_prepare_v2(hdb, delete_sql, -1, &delete_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_prepare_v2 failed: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_bind_int(delete_stmt, 1, id);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_int failed: %s\n",
	    sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_step(delete_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "ERROR: sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  /* Check the execution result
   */
  num_change = sqlite3_changes(hdb);
  if (!num_change) {
    scode = -ENOENT;  /* No such contact exists */
  }

out:
  sqlite3_finalize(delete_stmt);

  return scode;
}

/*
 * Deletes a contact from the address book database.
 */
int ab_delete_contact(ab_contact_t *contact) {
  int rc;

  rc = _db_delete_contact(contact->id);
  if (rc < 0)
    return -1;

  return 0;
}

/*
 * Deletes a contact with the given id from the address book database.
 */
int ab_delete_contact_v2(int id) {
  int rc;

  rc = _db_delete_contact(id);
  if (rc < 0)
    return -1;

  return 0;
}


/*
 * Retrieves a contact for the given id.
 *
 * [in] id The index of the contact in the table.
 * [out] pp_contact A pointer to a pointer to the retrieved contact.
 *
 * @return 0 if successful, -1 if it was not found.
 */
int ab_get_contact_by_id(int id, ab_contact_t **pp_contact) {
  int rc;
  int scode = 0;
  char const query[] = "SELECT * FROM contacts WHERE id=?";
  sqlite3_stmt *stmt = NULL;
  ab_contact_t *contact = NULL;

  rc = sqlite3_prepare_v2(hdb, query, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_prepare_v2 failed: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_bind_int(stmt, 1, id);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "ERROR: sqlite3_bind_int failed: %s\n",
	    sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
    fprintf(stderr, "ERROR: sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    scode = -1;
    goto out;
  }

  /* If rc is equal to SQLITE_ROW then a contact with the given id was found! */
  if (rc == SQLITE_ROW) {
    char const *psz = NULL;

    rc = ab_contact_create(&contact);
    if (rc < 0 || !contact) {
      scode = -1;
      goto out;
    }

    contact->id = sqlite3_column_int(stmt, 0);

    psz = (char const *)sqlite3_column_text(stmt, 1);
    contact->fname = xstrdup(psz);

    psz = (char const *)sqlite3_column_text(stmt, 2);
    contact->lname = xstrdup(psz);

    psz = (char const *)sqlite3_column_text(stmt, 3);
    contact->email = xstrdup(psz);

    *pp_contact = contact;
  }
  /* If rc is equal to SQLITE_DONE then NO contact with the given id was found */

out:
  sqlite3_finalize(stmt);

  return scode;
}
