#include <stdio.h>
#include <stdlib.h>

#include "temperature.h"

int
main(int argc, char **argv)
{
  int celcius = 0;
  int fahrenheit = 0;

  if (argc < 2)
    {
      fputs("usage: fahtocel <fahrenheit value>\n", stderr);
      return 1;
    }

  fahrenheit = atoi(argv[1]);

  celcius = temperature_fahrenheit_to_celcius(fahrenheit);
  printf("%d\n", celcius);

  return 0;
}
