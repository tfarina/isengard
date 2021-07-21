#include "sma.h"

/**
 * Calculates a simple moving average.
 *
 * Example:
 *
 * arr = [1, 2, 3, 4, 5, 6, 7, 8, 9]
 * outarr = [ '2.50', '3.50', '4.50', '5.50', '6.50', '7.50' ]
 *     //=>   │       │       │       │       │       └─(6+7+8+9)/4
 *     //=>   │       │       │       │       └─(5+6+7+8)/4
 *     //=>   │       │       │       └─(4+5+6+7)/4
 *     //=>   │       │       └─(3+4+5+6)/4
 *     //=>   │       └─(2+3+4+5)/4
 *     //=>   └─(1+2+3+4)/4
 */
void smovavg(double const *arr, size_t size, int period, double *outarr)
{
  int i;
  double sum = 0;

  /* Calculate the average for each of the first elements. */
  for (i = 0; i < period; i++) {
    sum += *(arr + i);

    *(outarr + i) = sum / (i + 1);
  }

  for (i = period; i < size; i++) {
    sum -= *(arr + i - period);  /* Remove the old elements from the sum */

    sum += *(arr + i);  /* Add the new (or most recent) elements to the sum */


    /* Calculate the average */
    *(outarr + i) = sum / period;
  }
}
