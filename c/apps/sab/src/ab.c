#include "ab.h"

#include <stdio.h>

#include "third_party/sqlite/amalgamation/sqlite3.h"

#include "fort.h"
#include "strutils.h"

static const char dbname[] = "abdb.sqlite3";
static sqlite3 *conn = NULL;

static sqlite3_stmt *insert_stmt;
static const char insert_sql[] =
  "INSERT INTO contacts (fname, lname, email) VALUES (?1, ?2, ?3);";

static sqlite3_stmt *update_stmt;
static const char update_sql[] =
  "UPDATE contacts SET fname=?2, lname=?3, email=?4 WHERE id=?1;";

static sqlite3_stmt *delete_stmt;
static const char delete_sql[] =
  "DELETE FROM contacts WHERE id=?1;";

static sqlite3_stmt *select_stmt;
static const char select_sql[] = "SELECT * FROM contacts";

static void _ab_sqlite_disconnect(sqlite3 *db) {
  if (sqlite3_close(db) != SQLITE_OK) {
    fprintf(stderr, "error closing SQLite database: %s\n", sqlite3_errmsg(db));
  }
}

/**
 * Makes sure the 'contacts' table is created if it does not exist yet.
 *
 * @return return 0 on success, -1 otherwise.
 */
static int _create_tables(void) {
  int rc;
  sqlite3_stmt *stmt;
  const char sql[] =
    "CREATE TABLE IF NOT EXISTS contacts ("
    "  id INTEGER PRIMARY KEY,"     /* id */
    "  fname TEXT,"                 /* first name */
    "  lname TEXT,"                 /* last name */
    "  email TEXT"                  /* email */
    ");";

  if (sqlite3_prepare(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing create statement: %s\n", sqlite3_errmsg(conn));
    _ab_sqlite_disconnect(conn);
    return -1;
  }

  rc = sqlite3_step(stmt);

  sqlite3_finalize(stmt);
  stmt = NULL;

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "error creating contacts table: %s\n", sqlite3_errmsg(conn));
    _ab_sqlite_disconnect(conn);
    return -1;
  }

  return 0;
}

ab_contact_t *ab_contact_alloc(void) {
  ab_contact_t *contact;

  contact = (ab_contact_t *) malloc(sizeof(*contact));
  if (contact == NULL) {
    return NULL;
  }

  contact->fname = NULL;
  contact->lname = NULL;
  contact->email = NULL;

  return contact;
}

void ab_contact_set_first_name(ab_contact_t *contact, char const *fname) {
  contact->fname = fname;
}

void ab_contact_set_last_name(ab_contact_t *contact, char const *lname) {
  contact->lname = lname;
}

void ab_contact_set_email(ab_contact_t *contact, char const *email) {
  contact->email = email;
}

char const *ab_contact_get_first_name(ab_contact_t *contact) {
  return contact->fname;
}

char const *ab_contact_get_last_name(ab_contact_t *contact) {
  return contact->lname;
}

char const *ab_contact_get_email(ab_contact_t *contact) {
  return contact->email;
}

static alpm_list_t *contact_list;

static void load_contacts(void) {
  while (sqlite3_step(select_stmt) == SQLITE_ROW) {
    ab_contact_t *contact = ab_contact_alloc();
    contact->id = sqlite3_column_int(select_stmt, 0);
    contact->fname = f_strdup((const char *)sqlite3_column_text(select_stmt, 1));
    contact->lname = f_strdup((const char *)sqlite3_column_text(select_stmt, 2));
    contact->email = f_strdup((const char *)sqlite3_column_text(select_stmt, 3));
    contact_list = alpm_list_add(contact_list, contact);
  }
}

int ab_init(void) {
  /* Do nothing if the database handle has been set. */
  if (conn) {
    return 0;
  }

  if (sqlite3_open(dbname, &conn) != SQLITE_OK) {
    fprintf(stderr, "error opening SQLite database %s: %s\n", dbname, sqlite3_errmsg(conn));
    sqlite3_close(conn);
    return -1;
  }

  if (_create_tables()) {
    _ab_sqlite_disconnect(conn);
    return -1;
  }

  if (sqlite3_prepare(conn, insert_sql, -1, &insert_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing insert statement: %s\n",
            sqlite3_errmsg(conn));
    _ab_sqlite_disconnect(conn);
    return -1;
  }

  if (sqlite3_prepare(conn, update_sql, -1, &update_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing update statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  if (sqlite3_prepare(conn, delete_sql, -1, &delete_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing delete statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  if (sqlite3_prepare(conn, select_sql, -1, &select_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing select statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  load_contacts();

  return 0;
}

int ab_close(void) {
  sqlite3_finalize(insert_stmt);
  insert_stmt = NULL;

  sqlite3_finalize(update_stmt);
  update_stmt = NULL;

  sqlite3_finalize(delete_stmt);
  delete_stmt = NULL;

  sqlite3_finalize(select_stmt);
  select_stmt = NULL;

  _ab_sqlite_disconnect(conn);
  conn = NULL;

  return 0;
}

int ab_add_contact(ab_contact_t *contact) {
  contact_list = alpm_list_add(contact_list, contact);

  sqlite3_bind_text(insert_stmt, 1, contact->fname, -1, SQLITE_STATIC);
  sqlite3_bind_text(insert_stmt, 2, contact->lname, -1, SQLITE_STATIC);
  sqlite3_bind_text(insert_stmt, 3, contact->email, -1, SQLITE_STATIC);

  if (sqlite3_step(insert_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error inserting into contacts table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(insert_stmt);

  return 0;
}

int ab_change_contact(ab_contact_t *contact) {
  sqlite3_bind_int(update_stmt, 1, contact->id);
  sqlite3_bind_text(update_stmt, 2, contact->fname, -1, SQLITE_STATIC);
  sqlite3_bind_text(update_stmt, 3, contact->lname, -1, SQLITE_STATIC);
  sqlite3_bind_text(update_stmt, 4, contact->email, -1, SQLITE_STATIC);

  if (sqlite3_step(update_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error updating contacts table: %s\n", sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(update_stmt);

  return 0;
}

static int _ab_contact_cmp(void const *p1, void const *p2) {
  ab_contact_t *c1 = (ab_contact_t *)p1;
  ab_contact_t *c2 = (ab_contact_t *)p2;
  return c1->id == c2->id;
}

int ab_delete_contact(ab_contact_t *contact) {
  void *vc;

  contact_list = alpm_list_remove(contact_list, contact, _ab_contact_cmp, &vc);

  sqlite3_bind_int(delete_stmt, 1, contact->id);

  if (sqlite3_step(delete_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error deleting contacts from table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(delete_stmt);

  return 0;
}

alpm_list_t *ab_get_contact_list(void) {
  return contact_list;
}

ab_contact_t *ab_get_contact_by_id(int id) {
  alpm_list_t *i;

  for (i = contact_list; i; i = alpm_list_next(i)) {
    ab_contact_t *contact = (ab_contact_t *)i->data;
    if (contact->id == id) {
      return contact;
    }
  }

  return NULL;
}

int ab_print_contact_records(void) {
  ft_table_t *table;
  alpm_list_t *i;

  ft_set_default_border_style(FT_BASIC2_STYLE);
  table = ft_create_table();
  ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
  ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);

  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_write_ln(table, "ID", "First Name", "Last Name", "Email");

  for (i = contact_list; i; i = alpm_list_next(i)) {
    ab_contact_t *contact = (ab_contact_t *)i->data;
    int length = snprintf(NULL, 0, "%d", contact->id);
    char* idstr = malloc(length + 1);
    snprintf(idstr, length + 1, "%d", contact->id);
    ft_write_ln(table, idstr, contact->fname, contact->lname, contact->email);
    free(idstr);
  }

  printf("%s\n", ft_to_string(table));
  ft_destroy_table(table);

  return 0;
}
