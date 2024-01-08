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
      fputs("usage: celtofah <celcius value>\n", stderr);
      return 1;
    }

  celcius = atof(argv[1]);

  fahrenheit = temperature_celcius_to_fahrenheit(celcius);
  printf("%.1f\n", fahrenheit);

  return 0;
}
