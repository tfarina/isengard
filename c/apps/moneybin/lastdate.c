#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "dba.h"
#include "ffileutils.h"
#include "futils.h"

#define USERCONFFILE ".moneybinrc"

/* https://github.com/BenjiWiebe/lctp/blob/master/datetime.c */
#define FMT_ERR ((time_t)(-1))

static time_t parse_date(char const *str, char sep)
{
  /* format (x is _sep_): yyyyxmmxdd */

  if (strlen(str) != 10)
    return FMT_ERR;

  char syear[5];
  char smonth[3];
  char sday[3];
  int year = 0, month = 0, day = 0;

  strncpy(syear, str, 4);
  syear[4] = 0;
  year = strtol(syear, NULL, 10);

  strncpy(smonth, str+5, 2);
  smonth[2] = 0;
  month = strtol(smonth, NULL, 10);

  strncpy(sday, str+8, 2);
  sday[2] = 0;
  day = strtol(sday, NULL, 10);

  if ((str[4] != sep) || (str[7] != sep))
    return FMT_ERR;

  /* format (x is _sep_): mmxddxyyyy */
  /*
  strncpy(smonth, str, 2);
  smonth[2] = 0;
  month = strtol(smonth, NULL, 10);

  strncpy(sday, str+3, 2);
  sday[2] = 0;
  day = strtol(sday, NULL, 10);

  strncpy(syear, str+6, 4);
  syear[4] = 0;
  year = strtol(syear, NULL, 10);

  if((str[2] != sep) || (str[5] != sep))
    return FMT_ERR;
  */

  struct tm *utm = calloc(1, sizeof(struct tm));
  utm->tm_mday = day;
  utm->tm_mon = month - 1;
  utm->tm_year = year - 1900;

  time_t t = mktime(utm);
  free(utm);

  if (t == -1)
    return FMT_ERR;

  return t;
}

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
  config_load_r(&config, userconffile);

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

  time_t ld;
  struct tm *tim;
  char last_date_str[12];

  ld = parse_date(lastdate, '-');
  tim = localtime(&ld);
  strftime(last_date_str, sizeof(last_date_str), "%F", tim);
  printf("%s\n", last_date_str);

  (void)dba_result_deinit(result);
  (void)dba_disconnect(handle);
  (void)dba_deinit(handle);
  
  return 0;
}
