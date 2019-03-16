#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
#include <curl/curl.h>

#include "buffer.h"
#include "curl_helper.h"
#include "ffileutils.h"
#include "strutils.h"

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

  if (argc != 2) {
    fputs("usage: csvdownload symbol\n", stderr);
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
  printf("Downloading file...\n\n");
  printf("Start Date: %s\n", start_date_str);
  printf("End Date: %s\n", end_date_str);
  printf("Frequency: Daily\n");

  download_quotes_from_yahoo(symbol, one_year_ago, now, &buf);

  /* TODO: do not print this by default. */
  printf("%s\n", buf.data);

  sprintf(filename, "%s.csv", symbol);
  f_write_file(filename, buf.data, buf.length);

  return 0;
}
