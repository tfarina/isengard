#include <stdio.h>
#include <string.h>

/* http://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html */

int
main(void)
{
  char symbol[10];
  char start_date[12];
  char end_date[12];

  printf("Enter the stock symbol of the company (e.g., MSFT): ");

  if (fgets(symbol, 10, stdin) == NULL) {
    return 1;
  }

  symbol[strcspn(symbol, "\n")] = 0;
  printf("You entered the stock symbol: %s\n", symbol);

  putc('\n', stdout);

  printf("Enter the starting date as YYYY-MM-DD: ");

  if (fgets(start_date, 12, stdin) == NULL) {
    return 1;
  }

  start_date[strcspn(start_date, "\n")] = 0;
  printf("You entered the starting date: %s\n", start_date);

  putc('\n', stdout);

  printf("Enter the ending date as YYYY-MM-DD: ");

  if (fgets(end_date, 12, stdin) == NULL) {
    return 1;
  }

  end_date[strcspn(end_date, "\n")] = 0;
  printf("You entered the ending date: %s\n", end_date);

  return 0;
}
