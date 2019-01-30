/*
 * Modified from http://zetcode.com/db/mysqlc/
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "iniparser.h"
#include "util.h"

#define USERCONFFILE ".experimentsrc"

static MYSQL *conn = NULL;

static int db_connect(const char *host, const char *user,
                      const char *password, const char *dbname)
{
  unsigned int port = 0;

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

  return 0;
}

static int product_add(const char *name, int quantity, double price)
{
  char query[256];

  sprintf(query, "INSERT INTO products (`productName`, `productLine`, `quantityInStock`, `buyPrice`) VALUES ('%s', 'Motorcycles', %d, %f)",
          name, quantity, price);

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql insert failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  return 0;
}

int main(int argc, char **argv) {
  const char *homedir;
  char *userconffile;
  dictionary *ini;
  const char *host;
  const char *user;
  const char *password;
  const char *dbname;

  homedir = f_get_home_dir();
  userconffile = f_build_filename(homedir, USERCONFFILE);
  ini = iniparser_load(userconffile);
  host = iniparser_getstring(ini, "mysql:host", NULL);
  user = iniparser_getstring(ini, "mysql:user", NULL);
  password = iniparser_getstring(ini, "mysql:password", NULL);
  dbname = iniparser_getstring(ini, "mysql:dbname", NULL);

  if (db_connect(host, user, password, dbname)) {
    return -1;
  }

  if (product_add("Ducati", 25, 8999.00) == -1) {
    return -1;
  }

  mysql_close(conn);

  return 0;
}
