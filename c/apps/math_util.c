#include "math_util.h"

#include <math.h>

long int bin2dec(long int num) {
  long int rem, sum = 0, power = 0;

  while (num > 0) {
    rem = num % 10;
    num = num / 10;
    sum = sum + rem * pow(2, power);
    power++;
  }

  return sum;
}
