#include <stddef.h>
#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

static double Sum(double a[], size_t size)
{
  double sum = 0.0;
  size_t i;

  for (i = 0; i < size; i++) {
    sum += a[i];
  }

  return sum;
}

static double Mean(double a[], size_t size)
{
  double sum = 0.0;

  sum = Sum(a, size);

  return sum / size;
}

int main(void)
{
  double a[] = {1, 2, 3};
  double sum = 0.0;
  double mean = 0.0;

  sum = Sum(a, ARRAY_SIZE(a));
  mean = Mean(a, ARRAY_SIZE(a));

  printf("%f\n", sum);
  printf("%f\n", mean);

  return 0;
}
