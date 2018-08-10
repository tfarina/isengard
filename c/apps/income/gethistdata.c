#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
#include <curl/curl.h>
#include <mysql/mysql.h>

#include "buffer.h"
#include "curl_helper.h"
#include "csv.h"
#include "csv_helper.h"
#include "iniparser.h"
#include "env.h"
#include "db_mysql.h"
#include "stock.h"
#include "file.h"

#define USERCONFFILE ".traderc"

int main(int argc, char *argv[])
{
  char *symbol;
  time_t start_date;
  time_t end_date;
  struct tm* tm;
  char start_date_str[12];
  char end_date_str[12];
  CURL *curl;
  CURLcode result;
  char histurl[255];
  FILE *fp;
  buffer_t buf;
  char fullurl[255];
  char filename[255];
  struct csv_parser parser;
  int rc;
  stock_info_t stock;
  size_t bytes_processed;
  char *homedir;
  char *userconffile;
  dictionary *ini;
  const char *host;
  const char *user;
  const char *password;
  const char *dbname;
  MYSQL *conn = NULL;
  size_t i;

  if (argc != 2) {
    fputs("usage: gethistdata symbol\n", stderr);
    return 1;
  }

  symbol = strdup(argv[1]);

  end_date = time(0);   // get time now (today).
  tm = localtime(&end_date);
  strftime(end_date_str, sizeof(end_date_str), "%F", tm);

  tm->tm_year = tm->tm_year - 1; // 1 year ago from today.
  strftime(start_date_str, sizeof(start_date_str), "%F", tm);
  start_date = mktime(tm);

  printf("Downloading historical data.\n\n");
  printf("Symbol: %s\n", symbol);
  printf("Start Date: %s\n", start_date_str);
  printf("End Date: %s\n", end_date_str);
  printf("Frequency: Daily\n");

  curl_global_init(CURL_GLOBAL_NOTHING);

  /* 1- Download history page to get the cookies. */
  curl = curl_easy_init();

  sprintf(histurl, "https://finance.yahoo.com/quote/%s/history", symbol);

  fp = fopen("history.html", "w");

  curl_easy_setopt(curl, CURLOPT_URL, histurl);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
  curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
            
  curl_easy_setopt(curl, CURLOPT_ENCODING, "");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_file);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

  result = curl_easy_perform(curl);
  fclose(fp);

  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    return -1;
  }

  buffer_init(&buf);

  /* 2- Download history page to memory to get the crumb. */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_memory);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buf);
            
  result = curl_easy_perform(curl);
            
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    return -1;
  }

  /* 3- Extract the crumb value from CrumbStore. */
  char crumb_key[] = "CrumbStore\":{\"crumb\":\"";
  char *ptr1 = strstr(buf.data, crumb_key);
  char *crumb = ptr1 + strlen(crumb_key);
  char *ptr3 = strstr(crumb, "\"}");
  if (ptr3 != NULL) {
    *ptr3 = '\0';
  }

  printf("crumb: %s\n", crumb);

  sprintf(fullurl,
         "https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%ld&period2=%ld&interval=1d&events=history&crumb=%s",
	  symbol, start_date, end_date, crumb);

  printf("url: %s\n", fullurl);

  free(buf.data);

  buffer_init(&buf);

  /* 4- With cookies and crumb, it is ready to download the historical data
   * (in csv format) to memory and save it to file.
   */
  curl_easy_setopt(curl, CURLOPT_URL, fullurl);
  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_memory);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buf);

  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    return -1;
  }

  printf("%s\n", buf.data);

  /* 5- Now proceed to save buf.data to file. */
  sprintf(filename, "%s.csv", symbol);
  write_file(filename, buf.data, buf.length);

  /* 6- Read the file to parse the csv and import it to the MySQL table. */
  rc = csv_init(&parser, CSV_APPEND_NULL);
  if (rc != 0) {
    fprintf(stderr, "failed to initialize CSV parser\n");
    return -1;
  }

  memset((void *)&stock, 0, sizeof(stock_info_t));
  stock.symbol = symbol;
  stock.ticks_capacity = 2;
  stock.ticks = malloc(stock.ticks_capacity * sizeof(stock_tick_t));
  if (stock.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for stock data\n",
            stock.ticks_capacity * sizeof(stock_tick_t));
    return -1;
  }
 
  bytes_processed = csv_parse(&parser, (void*)buf.data, buf.length,
                              process_field, process_row, &stock);
  rc = csv_fini(&parser, process_field, process_row, &stock);
 
  if (stock.error || rc != 0 || bytes_processed < buf.length) {
    fprintf(stderr,
            "read %zu bytes out of %zu: %s\n",
	    bytes_processed, buf.length, csv_strerror(csv_error(&parser)));
    return -1;
  }

  homedir = get_homedir();
  userconffile = make_file_path(homedir, USERCONFFILE);
  ini = iniparser_load(userconffile);
  if (ini == NULL) {
    fprintf(stderr, "Cannot read configuration file: %s\n", userconffile);
    return -1;
  }
  host = iniparser_getstring(ini, "mysql:host", NULL);
  user = iniparser_getstring(ini, "mysql:user", NULL);
  password = iniparser_getstring(ini, "mysql:password", NULL);
  dbname = iniparser_getstring(ini, "mysql:dbname", NULL);

  curl_easy_cleanup(curl);
  curl_global_cleanup();
 
  /* Connect to the database to start importing the data. */
  if ((conn = db_mysql_connect(host, user, password, dbname)) == NULL) {
    return -1;
  }

  iniparser_freedict(ini);

  printf("Importing records...\n");

  for (i = 0; i < stock.ticks_length; i++) {
    stock_tick_t *tick = stock.ticks + i;

    if (stock_add_tick(conn, &stock, tick) != -1) {
    }

    printf("date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);

    free(tick->date);
  }

  printf("%d rows imported\n", stock.ticks_length);

  free(stock.ticks);

  return 0;
}
