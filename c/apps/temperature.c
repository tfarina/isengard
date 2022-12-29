#include "temperature.h"

int
temperature_celcius_to_fahrenheit(int celcius)
{
  int result;

  result = ((celcius * 9) / 5) + 32;

  return result;
}

float
temperature_fahrenheit_to_celcius(float fahrenheit)
{
  float result;

  result = (fahrenheit - 32) * (5.0f / 9.0f);

  return result;
}
