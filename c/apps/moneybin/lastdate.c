#include <stdio.h>
#include <string.h>

#include "config.h"
#include "dba.h"
#include "ffileutils.h"
#include "futils.h"

#define USERCONFFILE ".moneybinrc"

int main(void)
{
  char const *homedir;
  char const *userconffile;
  config_t config;
  int rc;
  char query[256];
  dba_t *handle = NULL;
  dba_result_t *result = NULL;
  char const *lastdate;

  homedir = f_get_home_dir();
  userconffile = f_build_filename(homedir, USERCONFFILE);

  config_init(&config);
  config_load(&config, userconffile);

  rc = dba_init(&handle, config.database);
  if (rc < 0) {
    fprintf(stderr, "dba_init(): %s\n", dba_strerror(rc));
    return -1;
  }

  rc = dba_connect(handle, config.host, config.port, config.user, config.password, config.dbname);
  if (rc < 0) {
    fprintf(stderr, "dba_connect(): %s\n", dba_strerror(rc));
    (void)dba_deinit(handle);
    return -1;
  }

  sprintf(query, "SELECT max(date) FROM historicaldata");

  rc = dba_query(handle, query, 0);
  if (rc != DBA_SUCCESS) {
    fprintf(stderr, "dba_query(): %s\n", dba_strerror(rc));
    (void)dba_disconnect(handle);
    (void)dba_deinit(handle);
    return -1;
  }

  while ((rc = dba_result_init(handle, &result)) != DBA_RES_DONE) {
    if (rc < 0) {
      fprintf(stderr, "dba_result_init(): %s\n", dba_strerror(rc));
      (void)dba_disconnect(handle);
      (void)dba_deinit(handle);
      return -1;
    }

    switch (rc) {
    case DBA_RES_NOROWS:
      fprintf(stderr, "dba_result_init(): unexpected return value\n");
      (void)dba_disconnect(handle);
      (void)dba_deinit(handle);
      return -1;

    case DBA_RES_ROWS:
      while ((rc = dba_result_fetch_row(result)) != DBA_ROW_DONE) {
        if (rc < 0) {
          fprintf(stderr, "dba_result_fetch_row(): %s\n", dba_strerror(rc));
          (void)dba_disconnect(handle);
          (void)dba_deinit(handle);
          return -1;
	}

        lastdate = dba_result_get_field_value(result, 0);
        if (lastdate == NULL) {
          fprintf(stderr, "unexpected lastdate '%s'\n", dba_result_get_field_value(result, 0));
          (void)dba_disconnect(handle);
          (void)dba_deinit(handle);
          return -1;
	}
      }
      break;
    default:
      ;/*assert(0);*/
    }
  }

  printf("%s\n", lastdate);

  (void)dba_result_deinit(result);
  (void)dba_disconnect(handle);
  (void)dba_deinit(handle);
  
  return 0;
}
