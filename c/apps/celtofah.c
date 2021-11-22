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
      fputs("usage: celtofah <celcius value>\n", stderr);
      return 1;
    }

  celcius = atoi(argv[1]);

  fahrenheit = temperature_celcius_to_fahrenheit(celcius);
  printf("%d\n", fahrenheit);

  return 0;
}
