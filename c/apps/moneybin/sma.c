#include "sma.h"

int ta_sma(int size, double const *const *inputs,
           const int period, double *const *outputs) {
    const double *input = inputs[0];
    double *output = outputs[0];
    const double scale = 1.0 / period;

    if (period < 1) return -1;
    if (size <= period - 1) return 0;

    double sum = 0;

    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
    }

    *output++ = sum * scale;

    for (i = period; i < size; ++i) {
        sum += input[i];
        sum -= input[i-period];
        *output++ = sum * scale;
    }

    return 0;
}
