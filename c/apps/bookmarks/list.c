/*
 * Modified from http://zetcode.com/db/mysqlc/
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "iniparser.h"

#define USERCONFFILE ".bookmarksrc"
#define PATH_SEP '/'

static char *get_homedir(void)
{
  return getenv("HOME");
}

/* Code from msmtp:src/tools.c:get_filename */
static char *make_file_path(const char *directory, const char *name)
{
  char *path;
  size_t dirlen;

  dirlen = strlen(directory);
  path = malloc((dirlen + strlen(name) + 2) * sizeof(char));
  strcpy(path, directory);
  if (dirlen == 0 || path[dirlen - 1] != PATH_SEP) {
    path[dirlen++] = PATH_SEP;
  }
  strcpy(path + dirlen, name);
  return path;
}

int main(int argc, char **argv) {
  char *homedir;
  char *userconffile;
  dictionary *ini;
  const char *host;
  const char *user;
  const char *password;
  const char *dbname;
  MYSQL *conn = NULL;
  MYSQL_RES *res = NULL;
  unsigned int num_fields;
  MYSQL_ROW row;
  unsigned int port = 0;
  char query[256];
  size_t querylen;
  int i;

  homedir = get_homedir();
  userconffile = make_file_path(homedir, USERCONFFILE);
  ini = iniparser_load(userconffile);
  host = iniparser_getstring(ini, "mysql:host", NULL);
  user = iniparser_getstring(ini, "mysql:user", NULL);
  password = iniparser_getstring(ini, "mysql:password", NULL);
  dbname = iniparser_getstring(ini, "mysql:dbname", NULL);

  if ((conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql: unable to allocate memory for database connection.\n");
    return -1;
  }

  if (mysql_real_connect(conn, host, user, password, dbname, port,
                         NULL, 0) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  sprintf(query, "SELECT * FROM bookmarks");

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql select failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  if ((res = mysql_store_result(conn)) == NULL) {
    fprintf(stderr, "mysql: sql result retrieval failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  num_fields = mysql_num_fields(res);

  while ((row = mysql_fetch_row(res))) {
    for (i = 0; i < num_fields; i++) {
      printf("%s ", row[i] ? row[i] : "NULL");
    }
    printf("\n");
  }

  mysql_free_result(res);
  mysql_close(conn);

  return 0;
}
