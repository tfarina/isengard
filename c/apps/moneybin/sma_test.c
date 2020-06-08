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
  double data_in[] = { 0.61, 0.60, 0.63, 0.65, 0.69, 0.67, 0.70, 0.68, 0.62, 0.63, 0.61 };
  int data_size = ARRAY_SIZE(data_in);
  int period = 3;
  int start = 2;
  const int output_length = data_size - start;
  double *data_out;
  int err;

  print_array(data_in, data_size);

  printf("The start amount is: %d\n", start);

  data_out = malloc((unsigned int)output_length * sizeof(double));

  printf("The output length is: %d\n", output_length);

  const double *all_inputs[] = {data_in};
  double *all_outputs[] = {data_out};

  err = ta_sma(data_size, all_inputs, period, all_outputs);

  printf("The output data is: ");
  print_array(data_out, output_length);

  return 0;
}
