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

#define MAXURLLEN 256

static int verbose = 1;

static char *crumb;

/*https://github.com/jerryvig/cython-project1/blob/351aece6a910ecff8867708fa16155a6bc444217/compute_statistics.c#L53*/
static int get_crumb(const char *response_text, char *crumb) {
  const char *crumbstore = strstr(response_text, "CrumbStore");
  if (crumbstore == NULL) {
    fputs("Failed to find CrumbStore\n", stderr);
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
  char histurl[MAXURLLEN];
  char downloadurl[MAXURLLEN];
  int retval;

  result = curl_global_init(CURL_GLOBAL_DEFAULT);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed\n");
    return -1;
  }

  curl = curl_easy_init();
  if (curl == NULL) {
    fprintf(stderr, "curl_easy_init() failed\n");
    return -1;
  }

  buffer_init(&html);

  memset(histurl, 0, MAXURLLEN);
  snprintf(histurl, sizeof(histurl), "https://finance.yahoo.com/quote/%s/history", symbol);

  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
  curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "br, gzip");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_memory);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&html);
  curl_easy_setopt(curl, CURLOPT_URL, histurl);

  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    curl_easy_cleanup(curl);
    return -1;
  }

  crumb = (char*)malloc(MAXURLLEN * sizeof(char));
  memset(crumb, 0, MAXURLLEN);
  retval = get_crumb(html.data, crumb);
  if (retval) {
    return -1;
  }

  buffer_init(out_csv);

  memset(downloadurl, 0, MAXURLLEN);
  snprintf(downloadurl, sizeof(downloadurl),
         "https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%ld&period2=%ld&interval=1d&events=history&crumb=%s",
	  symbol, start_date, end_date, crumb);

  if (verbose) {
    printf("Download URL: %s\n\n", downloadurl);
  }

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)out_csv);
  curl_easy_setopt(curl, CURLOPT_URL, downloadurl);

  if (verbose) {
    printf("Downloading file...\n\n");
  }

  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    curl_easy_cleanup(curl);
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

  if (argc != 2) {
    fputs("usage: csvget symbol\n", stderr);
    return 1;
  }

  symbol = f_strdup(argv[1]);

  now = time(NULL); /* get time right now */
  now_tm = localtime(&now);
  strftime(end_date_str, sizeof(end_date_str), "%F", now_tm);

  now_tm->tm_year = now_tm->tm_year - 1; /* Calculate 1 year ago date */
  strftime(start_date_str, sizeof(start_date_str), "%F", now_tm);
  one_year_ago = mktime(now_tm);

  /* TODO: Write this into a log file instead. So it can be inspected after the program ends. */
  if (verbose) {
    printf("Start Date: %s\n", start_date_str);
    printf("End Date: %s\n", end_date_str);
    printf("Period: Daily\n\n");
  }

  download_quotes_from_yahoo(symbol, one_year_ago, now, &buf);

  /* TODO: do not print this by default. */
  if (verbose) {
    printf("%s\n", buf.data);
  }

  sprintf(filename, "%s.csv", symbol);
  f_write_file(filename, buf.data, buf.length);

  return 0;
}
