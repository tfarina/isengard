/*
 * Modified from http://zetcode.com/db/mysqlc/
 * sudo apt-get install libmysqlclient-dev
 * gcc mysql_version.c -o mysql_version `mysql_config --cflags --libs`
 * gcc -o mysql_version `mysql_config --cflags` mysql_version.c `mysql_config --libs`
 */

#include <stdio.h>
#include <mysql/mysql.h>

int main(int argc, char **argv) {
  printf("MySQL client version: %s\n", mysql_get_client_info());

  return 0;
}
