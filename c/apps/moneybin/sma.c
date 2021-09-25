#include "sma.h"

/**
 * Calculates a simple moving average.
 *
 * Example:
 *
 * input  = [1, 2, 3, 4, 5, 6, 7, 8, 9]
 * output = [ '2.50', '3.50', '4.50', '5.50', '6.50', '7.50' ]
 *     //=>   │       │       │       │       │       └─(6+7+8+9)/4
 *     //=>   │       │       │       │       └─(5+6+7+8)/4
 *     //=>   │       │       │       └─(4+5+6+7)/4
 *     //=>   │       │       └─(3+4+5+6)/4
 *     //=>   │       └─(2+3+4+5)/4
 *     //=>   └─(1+2+3+4)/4
 */
int ind_sma(double const *input, size_t size, int period, double *output)
{
  int i;
  double sum = 0;

  /* Calculate the average for each of the first elements. */
  for (i = 0; i < period; i++) {
    sum += *(input + i);

    *(output + i) = sum / (i + 1);
  }

  for (i = period; i < size; i++) {
    sum -= *(input + i - period);  /* Remove the old elements from the sum */

    sum += *(input + i);  /* Add the new (or most recent) elements to the sum */


    /* Calculate the average */
    *(output + i) = sum / period;
  }

  return 0;
}
