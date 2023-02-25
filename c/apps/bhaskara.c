#include <math.h>
#include <stdio.h>

static double
delta(double a, double b, double c)
{
  return pow(b, 2) - (4 * a * c);
}

static double
root(double b, double a)
{
  return -b / (2 * a);
}

static double
rootplus(double b, double delta, double a)
{
  return (-b + sqrt(delta)) / (2 * a);
}

static double
rootminus(double b, double delta, double a)
{
  return (-b - sqrt(delta)) / (2 * a);
}

int
main(int argc, char **argv)
{
  double a, b, c;
  double d;
  double x;
  double x1;
  double x2;

  a = 2;
  b = -3;
  c = -5;

  d = delta(a, b, c);

  printf("delta = %.0f\n", d);

  if (d == 0)
    {
      x = root(b, a);
      printf("x = %.1f\n", x);
    }
  else if (d > 0)
    {
      x1 = rootplus(b, d, a);
      x2 = rootminus(b, d, a);

      printf("x1 = %.1f\n", x1);
      printf("x2 = %.1f\n", x2);
    }

  return 0;
}
