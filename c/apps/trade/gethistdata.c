#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
#include <curl/curl.h>
#include <mysql/mysql.h>

#include "buffer.h"
#include "csv.h"
#include "iniparser.h"
#include "env.h"
#include "db_mysql.h"
#include "stock.h"
 
static size_t write_data_to_file(void *data, size_t size, size_t nmemb, void *stream)
{
  size_t written;

  written = fwrite(data, size, nmemb, (FILE *)stream);

  return written;
}

static size_t write_data_to_memory(void *data, size_t size, size_t nmemb, void *ctx)
{
    buffer_t *buf = (buffer_t *)ctx;
    size_t realsize = size * nmemb;

    buf->data = (char *)realloc(buf->data, buf->length + realsize + 1);
    if (buf->data) {
        memcpy(&(buf->data[buf->length]), data, realsize);
        buf->length += realsize;
        buf->data[buf->length] = 0;
    }

    return realsize;
}

time_t date_to_epoch(const char *date) {
  struct tm time;
  time_t epoch_time;

  strptime(date, "%D", &time);

  printf("year: %d; month: %d; day: %d;\n",
	 time.tm_year, time.tm_mon, time.tm_mday);
  time.tm_isdst = -1;

  epoch_time = mktime(&time);

  return epoch_time;
}

static int write_file(const char *filename, const char *data, size_t size)
{
  int fd = creat(filename, 0666);
  if (fd < 0) {
    return -1;
  }

  ssize_t bytes_written_total = 0;
  for (ssize_t bytes_written_partial = 0; bytes_written_total < size;
       bytes_written_total += bytes_written_partial) {
    bytes_written_partial = write(fd, data + bytes_written_total,
                                      size - bytes_written_total);
    if (bytes_written_partial < 0) {
      return -1;
    }
  }

  if (close(fd) < 0) {
    return -1;
  }

  return size;
}

void process_field(void *field,
		   size_t field_len __attribute__((unused)),
		   void *ctx)
{
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;

  stock_data_t *cur_tick = stock->ticks + stock->ticks_used;
 
  // used for parsing floating-point values:
  // (declaring these in a switch/case is annoying)
  char *endptr;
  double dval;

  // Skip the first (header) line?
  if (strcmp((char*)field, "Date") == 0 ||
      strcmp((char*)field, "Open") == 0 ||
      strcmp((char*)field, "High") == 0 ||
      strcmp((char*)field, "Low") == 0 ||
      strcmp((char*)field, "Close") == 0 ||
      strcmp((char*)field, "Adj Close") == 0 ||
      strcmp((char*)field, "Volume") == 0) {
    return;
  }
 
  switch (stock->cur_field) {
  case DATE:
    // start of a new record; check if we need to reallocate
    if (stock->ticks_used == stock->ticks_alloc) {
      stock->ticks_alloc *= 2;
      stock->ticks = realloc(stock->ticks,
                             sizeof(stock_data_t) * stock->ticks_alloc);
      if (stock->ticks == NULL) {
        fprintf(stderr,
                "failed to reallocate %zu bytes for stock data: ",
		sizeof(stock_data_t) * stock->ticks_alloc);
        perror(NULL);
        stock->error = 1;
	return;
      }
      cur_tick = stock->ticks + stock->ticks_used;
    }
 
    // anyway, we just got tick data
    cur_tick->date = strdup((char*)field);
    break;
 
  case OPEN:
  case HIGH:
  case LOW:
  case CLOSE:
  case ADJ_CLOSE:
    if (strcmp((char*)field, "null") != 0) {
      dval = strtod((char*)field, &endptr);
      if (*endptr != '\0') {
        fprintf(stderr,
                "non-float value in record %zu, field %u: \"%s\"\n",
                 stock->ticks_used + 1, stock->cur_field + 1, field);
        stock->error = 1;
        return;
      }
 
      if (stock->cur_field == OPEN)
        cur_tick->open = dval;
      else if (stock->cur_field == HIGH)
        cur_tick->high = dval;
      else if (stock->cur_field == LOW)
        cur_tick->low = dval;
      else if (stock->cur_field == CLOSE)
        cur_tick->close = dval;
      else if (stock->cur_field == ADJ_CLOSE)
        cur_tick->adj_close = dval;
    }
  case VOLUME:
    cur_tick->volume = atoi((char*)field);
  }
 

  if (stock->cur_field == VOLUME) stock->ticks_used++;
  stock->cur_field = (stock->cur_field + 1) % 7;
}
 
void process_row(int delim __attribute__((unused)), void *ctx) {
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;
 
  if (stock->cur_field != DATE) {
    fprintf(stderr, "not enough fields in row %zu\n", stock->ticks_used);
    stock->error = 1;
  }
}

#define USERCONFFILE ".traderc"
#define PATH_SEP '/'

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

int main(int argc, char *argv[])
{
  char *symbol;
  char filename[255];
  char histurl[255];
  CURL *curl;
  CURLcode result;
  FILE *fp;
  buffer_t buf;
  char fullurl[255];
  time_t start_date;
  time_t end_date;
  struct tm* tm;
  char start_date_str[12];
  char end_date_str[12];
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

  sprintf(histurl, "https://finance.yahoo.com/quote/%s/history", symbol);

  end_date = time(0);   // get time now (today)
  tm = localtime(&end_date);
  strftime(end_date_str, sizeof(end_date_str), "%F", tm);

  tm->tm_year = tm->tm_year - 1; // 1 ago from today
  strftime(start_date_str, sizeof(start_date_str), "%F", tm);
  start_date = mktime(tm);

  buf.data = NULL;
  buf.length = 0;

  printf("Downloading historical data.\n\n");
  printf("Symbol: %s\n", symbol);
  printf("Start Date: %s\n", start_date_str);
  printf("End Date: %s\n", end_date_str);
  printf("Frequency: Daily\n");

  curl_global_init(CURL_GLOBAL_NOTHING);

  /* 1- Download html page to extract the crumb value from CrumbStore. */
  curl = curl_easy_init();

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

  /* 2- Download the html page to memory to extract the crumb. */
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
  buf.data = NULL;
  buf.length = 0;

  /* 4- With cookies and crumb, it is ready to download the historical data
   * (in csv format) to memory.
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

  // NOW I WANT TO SAVE buf.data to SYMBOL.csv and INSERT INTO MYSQL DATABASE.
  sprintf(filename, "%s.csv", symbol);
  write_file(filename, buf.data, buf.length);

  // Now that we have downloaded the historical data, parse the csv content
  // to store it in the MySQL table. 
  rc = csv_init(&parser, CSV_APPEND_NULL);
  if (rc != 0) {
    fprintf(stderr, "failed to initialize CSV parser\n");
    return -1;
  }

  memset((void *)&stock, 0, sizeof(stock_info_t));
  stock.symbol = symbol;
  stock.ticks_alloc = 2;
  stock.ticks = malloc(stock.ticks_alloc * sizeof(stock_data_t));
  if (stock.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for stock data\n",
            stock.ticks_alloc * sizeof(stock_data_t));
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
  host = iniparser_getstring(ini, "mysql:host", NULL);
  user = iniparser_getstring(ini, "mysql:user", NULL);
  password = iniparser_getstring(ini, "mysql:password", NULL);
  dbname = iniparser_getstring(ini, "mysql:dbname", NULL);

  printf("%s\n", dbname);

  curl_easy_cleanup(curl);
  curl_global_cleanup();
 
  if ((conn = db_mysql_connect(host, user, password, dbname)) == NULL) {
    return -1;
  }

  printf("Importing records...\n");

  for (i = 0; i < stock.ticks_used; i++) {
    stock_data_t *tick = stock.ticks + i;

    if (stock_add_tick(conn, stock.symbol, tick) != -1) {
    }

    printf("date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);

    free(tick->date);
  }

  printf("%d rows imported\n", stock.ticks_used);

  free(stock.ticks);

  return 0;
}
