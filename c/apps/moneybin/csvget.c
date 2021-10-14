#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
#include <curl/curl.h>

#include "buffer.h"
#include "curl_write_callbacks.h"
#include "ffileutils.h"
#include "fstrutils.h"
#include "iso8601.h"

#define BUFSIZE 128
#define MAXURLLEN 256

static int verbose = 1;

static char const *frequency[] = { "Daily", "Weekly", "Monthly", (char *) 0 };

/*https://github.com/jerryvig/cython-project1/blob/351aece6a910ecff8867708fa16155a6bc444217/compute_statistics.c#L53*/
/**
 * In @a response_text we should see a text like this near CrumbStore:
 * "CrumbStore\":{\"crumb\":\"k2MN42m9KC\\u002F\"
 */
static int get_crumb(const char *response_text, char *crumb) {
  const char *crumbstore = strstr(response_text, "CrumbStore");
  if (crumbstore == NULL) {
    fputs("Failed to find CrumbStore\n", stderr);
    return -1;
  }

  const char *colon_quote = strstr(crumbstore, ":\"");
  const char *end_quote = strstr(&colon_quote[2], "\"");
  char crumbclean[BUFSIZE];
  memset(crumbclean, 0, BUFSIZE);
  strncpy(crumb, &colon_quote[2], strlen(&colon_quote[2]) - strlen(end_quote));
  const char *twofpos = strstr(crumb, "\\u002F");

  if (twofpos) {
    strncpy(crumbclean, crumb, twofpos - crumb);
    strcat(crumbclean, "%2F");
    strcat(crumbclean, &twofpos[6]);
    memset(crumb, 0, BUFSIZE);
    strcpy(crumb, crumbclean);
  }

  return 0;
}

static int http_get(char const *url, buffer_t *outbuf)
{
  CURL *curl;
  CURLcode result;
  int response_code;

  curl = curl_easy_init();
  if (curl == NULL) {
    fprintf(stderr, "curl_easy_init() failed\n");
    return -1;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_memory_cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)outbuf);

  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    curl_easy_cleanup(curl);
    return -1;
  }

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  if (response_code != 200) {
    fprintf(stderr, "http_get: curl_easy_perform() returned code: %d", response_code);
    curl_easy_cleanup(curl);
    return -1;
  }

  curl_easy_cleanup(curl);

  return 0;
}

static int download_history_from_yahoo(char const *symbol, time_t start_date, time_t end_date, buffer_t *out_csv)
{
  CURL *curl;
  CURLcode result;
  buffer_t html;
  char histurl[MAXURLLEN];
  char downloadurl[MAXURLLEN];
  char *crumb;
  int retval;

  buffer_init(&html);

  memset(histurl, 0, MAXURLLEN);
  snprintf(histurl, sizeof(histurl), "https://finance.yahoo.com/quote/%s/history", symbol);

  http_get(histurl, &html);

  crumb = malloc(sizeof(char) * BUFSIZE);
  memset(crumb, 0, BUFSIZE);
  retval = get_crumb(html.data, crumb);
  if (retval < 0) {
    return -1;
  }

#if 0
  printf("crumb %s\n", crumb);
#endif

  buffer_init(out_csv);

  /*
   * Valid intervals: 1m,2m,5m,15m,30m,60m,90m,1h,1d,5d,1wk,1mo,3mo
   * Intraday data cannot extend last 60 days
   */
  memset(downloadurl, 0, MAXURLLEN);
  snprintf(downloadurl, sizeof(downloadurl),
         "https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%ld&period2=%ld&interval=1d&events=history&crumb=%s",
	  symbol, start_date, end_date, crumb);

  if (verbose) {
    printf("Download URL: %s\n\n", downloadurl);
  }

  http_get(downloadurl, out_csv);

  return 0;
}

int main(int argc, char *argv[])
{
  char *symbol;
  time_t now;
  time_t one_year_ago;
  struct tm* now_tm;
  struct tm* start_tm;
  char start_date_str[12];
  char end_date_str[12];
  buffer_t buf;
  char filename[255];
  CURLcode result;
  int retval;

  if (argc != 2) {
    fputs("usage: csvget symbol\n", stderr);
    return 1;
  }

  symbol = f_strdup(argv[1]);

  now = time(NULL); /* get time right now */
  now_tm = start_tm = localtime(&now);

  start_tm->tm_year = now_tm->tm_year - 1; /* Calculate 1 year ago date */
  one_year_ago = mktime(start_tm);

  /* TODO: Write this into a log file instead. So it can be inspected after the program ends. */
  if (verbose) {
    iso8601_to_str(one_year_ago, start_date_str, sizeof(start_date_str));
    iso8601_to_str(now, end_date_str, sizeof(end_date_str));

    printf("Start Date: %s\n", start_date_str);
    printf("End Date: %s\n", end_date_str);
    printf("Frequency: %s\n\n", frequency[0]);
  }

  result = curl_global_init(CURL_GLOBAL_DEFAULT);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed\n");
    return 1;
  }

  retval = download_history_from_yahoo(symbol, one_year_ago, now, &buf);
  if (retval < 0) {
    return 1;
  }

  curl_global_cleanup();

  /* TODO: do not print this by default. */
  if (verbose) {
    printf("%s\n", buf.data);
  }

  strcpy(filename, symbol);
  strcat(filename, ".csv");
  f_write_file(filename, buf.data, buf.size);

  return 0;
}
