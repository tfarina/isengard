#include <stdio.h>
#include <stdlib.h>

#include "temperature.h"

int
main(int argc, char **argv)
{
  float celcius = 0;
  float fahrenheit = 0;

  if (argc < 2)
    {
      fputs("usage: fahtocel <fahrenheit value>\n", stderr);
      return 1;
    }

  fahrenheit = atof(argv[1]);

  celcius = temperature_fahrenheit_to_celcius(fahrenheit);
  printf("%f\n", celcius);

  return 0;
}
