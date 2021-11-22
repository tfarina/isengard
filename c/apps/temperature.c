#include "temperature.h"

int
temperature_celcius_to_fahrenheit(int celcius)
{
  int result;

  result = ((celcius * 9) / 5) + 32;

  return result;
}

int
temperature_fahrenheit_to_celcius(int fahrenheit)
{
  int result;

  result = (fahrenheit - 32) * 5;
  result = result / 9;

  return result;
}
