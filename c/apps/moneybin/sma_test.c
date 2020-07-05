#include <stdio.h>
#include <stdlib.h>

#include "arraysize.h"
#include "sma.h"

static void print_array(double const *arr, size_t const size)
{
  size_t i;

  for (i = 0; i < size; i++) {
    if (i) {
      printf(", ");
    }
    printf("%.2f", *(arr + i));
  }

  printf("\n\n");
}

int main(void) {
  double indicator_data[] = { 0.61, 0.60, 0.63, 0.65, 0.69, 0.67, 0.70, 0.68, 0.62, 0.63, 0.61 };
  int period = 3;
  int start = 2;
  const int output_length = ARRAY_SIZE(indicator_data) - start;
  double *indicator_result;
  int err;

  print_array(indicator_data, ARRAY_SIZE(indicator_data));

  printf("The start amount is: %d\n", start);

  indicator_result = malloc((unsigned int)output_length * sizeof(double));

  printf("The output length is: %d\n", output_length);

  const double *all_inputs[] = {indicator_data};
  double *all_outputs[] = {indicator_result};

  err = ta_sma(ARRAY_SIZE(indicator_data), all_inputs, period, all_outputs);

  printf("The output data is: ");
  print_array(indicator_result, output_length);

  return 0;
}
