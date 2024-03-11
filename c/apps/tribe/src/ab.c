#include "ab.h"

#include <stdio.h>
#include <string.h>

#include "third_party/sqlite/amalgamation/sqlite3.h"

#include "os_path.h"
#include "xstring.h"

/* NOTE: Always increment this when you make incompatible db structure
 * changes */
#define DB_VERSION 0

static char const dbname[] = "abdb.sqlite3";
static sqlite3 *hdb = NULL;  /* SQLite db handle */

static alpm_list_t *contact_list;

static int _db_close(void) {
  int rc;

  rc = sqlite3_close(hdb);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to close the SQLite database: %s\n", sqlite3_errmsg(hdb));
    return -1;
  } else {
    hdb = NULL;
    /*if (debug) {
      printf("Database closed\n");
    }*/
  }

  return 0;
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
    fprintf(stderr, "sqlite3_exec failed: %s\n", err_msg);
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

int ab_init(char *dbpath) {
  char *dbfile;  /* Database filename */
  int rc;
  int corrupted = 0;
  int user_version = 0;
  char *err_msg = NULL;

  /* Do nothing if the database handle has been set. */
  if (hdb) {
    return 0;
  }

  dbfile = os_path_join(dbpath, dbname);

  rc = sqlite3_open(dbfile, &hdb);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to open the SQLite database at %s: %s\n", dbfile, sqlite3_errmsg(hdb));
    free(dbfile);
    sqlite3_close(hdb);
    hdb = NULL;
    return -1;
  }
  free(dbfile);


  rc = sqlite3_exec(hdb, "PRAGMA integrity_check;", _db_pragma_integrity_check_cb, &corrupted, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "sqlite3_exec failed: %s\n", err_msg);
    sqlite3_free(err_msg);
    return -1;
  }

  if (1 == corrupted) {
    fprintf(stderr, "Error: The SQLite database integrity check failed. It may be corrupted.\n");
    return -1;
  }

  rc = sqlite3_exec(hdb, "PRAGMA user_version;", _db_pragma_user_version_cb, &user_version, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "sqlite3_exec failed: %s\n", err_msg);
    sqlite3_free(err_msg);
    return -1;
  }

  rc = _db_init_schema();
  if (rc < 0) {
    _db_close();
    return -1;
  }

  return 0;
}

int ab_fini(void) {
  int rc;

  rc = _db_close();
  if (rc < 0) {
    return -1;
  }

  return 0;
}

int _db_get_contact_list(int *pCount, ab_contact_t **ppContacts) {
  int rc;
  int errcode = 0;
  char const count_sql[] = "SELECT COUNT(*) FROM contacts";
  sqlite3_stmt *count_stmt;
  int row_count = 0;
  char const select_sql[] = "SELECT * FROM contacts";
  sqlite3_stmt *select_stmt;
  ab_contact_t *contacts;
  int i;

  if (NULL == pCount || NULL == ppContacts) {
    return -1;  /* Invalid args */
  }

  rc = sqlite3_prepare_v2(hdb, count_sql, -1, &count_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the select count(*) statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_column_count(count_stmt);
  if (rc != 1) {
    return -1;
  }

  rc = sqlite3_step(count_stmt);
  if (rc == SQLITE_ROW) {
    row_count = sqlite3_column_int(count_stmt, 0);
  }

  rc = sqlite3_finalize(count_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the select count(*) statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  contacts = malloc(row_count * sizeof(ab_contact_t));
  if (NULL == contacts) {
    return -1;
  }

  rc = sqlite3_prepare_v2(hdb, select_sql, -1, &select_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the select statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  for (i = 0; i < row_count; i++) {
    if (sqlite3_step(select_stmt) == SQLITE_ROW) {
      contacts[i].id = sqlite3_column_int(select_stmt, 0);

      contacts[i].fname = xstrdup((const char *)sqlite3_column_text(select_stmt, 1));
      contacts[i].lname = xstrdup((const char *)sqlite3_column_text(select_stmt, 2));
      contacts[i].email = xstrdup((const char *)sqlite3_column_text(select_stmt, 3));
    }
  }

  *pCount = row_count;
  *ppContacts = contacts;

  rc = sqlite3_finalize(select_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the select statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
  }
  select_stmt = NULL;

  return errcode;
}

int ab_load_contacts(alpm_list_t **pp_contact_list) {
  int rc;
  int num_contacts = 0;
  ab_contact_t *contacts = NULL;
  int i;

  rc = _db_get_contact_list(&num_contacts, &contacts);

  for (i = 0; i < num_contacts; i++) {
    contact_list = alpm_list_add(contact_list, &contacts[i]);
  }

  *pp_contact_list = contact_list;

  return rc;
}

int _db_insert_contact(ab_contact_t *contact) {
  int rc;
  int errcode = 0;
  char const insert_sql[] =
      "INSERT INTO contacts (fname, lname, email) VALUES (?, ?, ?)";
  sqlite3_stmt *insert_stmt;

  rc = sqlite3_prepare_v2(hdb, insert_sql, -1, &insert_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the insert statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_bind_text(insert_stmt, 1, contact->fname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind fname parameter for insert statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(insert_stmt, 2, contact->lname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind lname parameter for insert statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(insert_stmt, 3, contact->email, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind email parameter for insert statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  rc = sqlite3_step(insert_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
  }

  sqlite3_reset(insert_stmt);

  rc = sqlite3_finalize(insert_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the insert statement: %s\n",
            sqlite3_errmsg(hdb));
  }
  insert_stmt = NULL;

out:
  if (insert_stmt) {
    sqlite3_reset(insert_stmt);
    sqlite3_finalize(insert_stmt);
  }

  return errcode;
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

int _db_update_contact(ab_contact_t* contact) {
  int rc;
  int errcode = 0;
  char const update_sql[] =
      "UPDATE contacts SET fname=?, lname=?, email=? WHERE id=?";
  sqlite3_stmt *update_stmt;

  rc = sqlite3_prepare_v2(hdb, update_sql, -1, &update_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the update statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_bind_text(update_stmt, 1, contact->fname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind fname parameter for update statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(update_stmt, 2, contact->lname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind lname parameter for update statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(update_stmt, 3, contact->email, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind email parameter for update statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_int(update_stmt, 4, contact->id);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind id parameter for update statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  rc = sqlite3_step(update_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "sqlite3_step failed: %s\n",
	    sqlite3_errmsg(hdb));
    errcode = -1;
  }

  sqlite3_reset(update_stmt);

  rc = sqlite3_finalize(update_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the update statement: %s\n",
            sqlite3_errmsg(hdb));
  }
  update_stmt = NULL;

out:
  if (update_stmt) {
    sqlite3_reset(update_stmt);
    sqlite3_finalize(update_stmt);
  }

  return errcode;
}

/*
 * Updates an existing contact in the address book database.
 */
int ab_update_contact(ab_contact_t *contact) {
  return _db_update_contact(contact);
}

static int _ab_contact_cmp(void const *p1, void const *p2) {
  ab_contact_t *c1 = (ab_contact_t *)p1;
  ab_contact_t *c2 = (ab_contact_t *)p2;
  return c1->id == c2->id;
}

static int _db_delete_contact(int id) {
  int rc;
  char const delete_sql[] = "DELETE FROM contacts WHERE id=?";
  sqlite3_stmt *delete_stmt;
  int rows_deleted;

  rc = sqlite3_prepare_v2(hdb, delete_sql, -1, &delete_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the delete statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_bind_int(delete_stmt, 1, id);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind id parameter for delete statement: %s\n",
	    sqlite3_errmsg(hdb));
    goto out;
  }

  rc = sqlite3_step(delete_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    goto out;
  }

  sqlite3_reset(delete_stmt);

  rc = sqlite3_finalize(delete_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the delete statement: %s\n",
            sqlite3_errmsg(hdb));
  }
  delete_stmt = NULL;

  rows_deleted = sqlite3_changes(hdb);

  return 0;

out:
  if (delete_stmt) {
    sqlite3_reset(delete_stmt);
    sqlite3_finalize(delete_stmt);
  }

  return -1;
}

/*
 * Deletes a contact from the address book database.
 */
int ab_delete_contact(ab_contact_t *contact) {
  void *vc;
  int rc;

  rc = _db_delete_contact(contact->id);
  if (rc < 0)
    return -1;

  contact_list = alpm_list_remove(contact_list, contact, _ab_contact_cmp, &vc);

  return 0;
}

/*
 * Deletes a contact with the given id from the address book database.
 */
int ab_delete_contact_v2(int id, int *pb_deleted) {
  int rc;
  char *query;
  char *err_msg;
  int rows_deleted;

  query = sqlite3_mprintf("DELETE FROM contacts WHERE id=%d;", id);
  rc = sqlite3_exec(hdb, query, NULL, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "sqlite3_exec failed: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_free(query);

    return -1;
  }

  rows_deleted = sqlite3_changes(hdb);
  if (rows_deleted > 0)
    *pb_deleted = 1;

  sqlite3_free(query);

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
  char const query[] = "SELECT * FROM contacts WHERE id=?";
  sqlite3_stmt *stmt;
  ab_contact_t *contact;
  int errcode = 0;

  rc = sqlite3_prepare_v2(hdb, query, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the select statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_bind_int(stmt, 1, id);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind id parameter for select statement: %s\n",
	    sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
    fprintf(stderr, "sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
    goto out;
  }

  /* If rc is equal to SQLITE_ROW then a contact with the given id was found! */
  if (rc == SQLITE_ROW) {
    contact = malloc(sizeof(ab_contact_t));
    if (NULL == contact) {
      errcode = -1;
      goto out;
    }

    contact->id = sqlite3_column_int(stmt, 0);
    contact->fname = xstrdup(sqlite3_column_text(stmt, 1));
    contact->lname = xstrdup(sqlite3_column_text(stmt, 2));
    contact->email = xstrdup(sqlite3_column_text(stmt, 3));

    *pp_contact = contact;
  }
  /* If rc is equal to SQLITE_DONE then NO contact with the given id was found */

  rc = sqlite3_finalize(stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the select statement: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
  }

out:
  return errcode;
}
