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
#include "third_party/iniparser/iniparser.h"
#include "env.h"
#include "db_mysql.h"
#include "stock.h"
#include "file.h"

#define USERCONFFILE ".traderc"

static char *crumb;

/*https://github.com/jerryvig/cython-project1/blob/351aece6a910ecff8867708fa16155a6bc444217/compute_statistics.c#L53*/
static int get_crumb(const char *response_text, char *crumb) {
  const char *crumbstore = strstr(response_text, "CrumbStore");
  if (crumbstore == NULL) {
    puts("Failed to find crumbstore....");
    return 1;
  }
  const char *colon_quote = strstr(crumbstore, ":\"");
  const char *end_quote = strstr(&colon_quote[2], "\"");
  char crumbclean[128];
  memset(crumbclean, 0, 128);
  strncpy(crumb, &colon_quote[2], strlen(&colon_quote[2]) - strlen(end_quote));
  const char *twofpos = strstr(crumb, "\\u002F");

  if (twofpos) {
    strncpy(crumbclean, crumb, twofpos - crumb);
    strcat(crumbclean, "%2F");
    strcat(crumbclean, &twofpos[6]);
    memset(crumb, 0, 128);
    strcpy(crumb, crumbclean);
  }
  return 0;
}

static int download_quotes_from_yahoo(char *symbol, time_t start_date, time_t end_date, buffer_t *out_csv)
{
  CURL *curl;
  CURLcode result;
  buffer_t html;
  char histurl[128];
  char downloadurl[256];

  curl_global_init(CURL_GLOBAL_NOTHING);

  /* 1. Write history page into file and get the cookies.txt file (they
   * will be used later on).
   */
  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
  curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "br, gzip");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_memory);

  buffer_init(&html);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&html);

  memset(histurl, 0, 128);
  sprintf(histurl, "https://finance.yahoo.com/quote/%s/history", symbol);
  curl_easy_setopt(curl, CURLOPT_URL, histurl);

  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    return -1;
  }

  crumb = (char*)malloc(128 * sizeof(char));
  memset(crumb, 0, 128);
  int crumb_failure = get_crumb(html.data, crumb);
  if (crumb_failure) {
    return -1;
  }

  buffer_init(out_csv);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)out_csv);

  memset(downloadurl, 0, 256);
  sprintf(downloadurl,
         "https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%ld&period2=%ld&interval=1d&events=history&crumb=%s",
	  symbol, start_date, end_date, crumb);
  printf("downloadurl = %s\n", downloadurl);
  curl_easy_setopt(curl, CURLOPT_URL, downloadurl);

  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    return -1;
  }

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return 0;
}

int main(int argc, char *argv[])
{
  char *symbol;
  time_t now;
  time_t one_year_ago;
  struct tm* now_tm;
  char start_date_str[12];
  char end_date_str[12];
  buffer_t buf;
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

  now = time(NULL); /* get time right now */
  now_tm = localtime(&now);
  strftime(end_date_str, sizeof(end_date_str), "%F", now_tm);

  now_tm->tm_year = now_tm->tm_year - 1; /* Calculate 1 year ago date */
  strftime(start_date_str, sizeof(start_date_str), "%F", now_tm);
  one_year_ago = mktime(now_tm);

  /* TODO: Write this into a log file instead. So it can be inspected after the program ends. */
  printf("Downloading file...\n\n");
  printf("Start Date: %s\n", start_date_str);
  printf("End Date: %s\n", end_date_str);
  printf("Frequency: Daily\n");

  download_quotes_from_yahoo(symbol, one_year_ago, now, &buf);

  /* TODO: This function should stop here and return buf! */
  printf("%s\n", buf.data);

  /* 5. Now write |buf.data| to file. */
  sprintf(filename, "%s.csv", symbol);
  write_file(filename, buf.data, buf.length);

  /* 6. Parse it. */
  if (csv_init(&parser, CSV_APPEND_NULL) != 0) {;
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
                              csv_column_cb, csv_row_cb, &stock);
  rc = csv_fini(&parser, csv_column_cb, csv_row_cb, &stock);
 
  if (stock.error || rc != 0 || bytes_processed < buf.length) {
    fprintf(stderr,
            "read %zu bytes out of %zu: %s\n",
	    bytes_processed, buf.length, csv_strerror(csv_error(&parser)));
    return -1;
  }

  /* 7. Import the data into MySQL. */
  homedir = get_home_dir();
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

  /* 8. Connect to the database to start importing the data. */
  if ((conn = db_mysql_connect(host, user, password, dbname)) == NULL) {
    return -1;
  }

  iniparser_freedict(ini);

  printf("Importing records...\n");

  for (i = 0; i < stock.ticks_length; i++) {
    stock_tick_t *tick = stock.ticks + i;

    if (stock_add_tick(conn, &stock, tick) != -1) {
    }
  }

  printf("Imported %d items for %s\n", stock.ticks_length, stock.symbol);

  free(stock.ticks);

  return 0;
}
