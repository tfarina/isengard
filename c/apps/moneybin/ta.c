#include "ta.h"

#include <stdlib.h>

ta_bars_t *
ta_bars_create(size_t numrows)
{
  ta_bars_t *bars;

  bars = malloc(sizeof(*bars));
  if (bars == NULL) {
    return NULL;
  }

  bars->numrows = numrows;

  bars->timestamp = malloc(sizeof(timestamp_t) * numrows);
  if (bars->timestamp == NULL) {
    free(bars);
    return NULL;
  }

  bars->open = malloc(sizeof(double) * numrows);
  if (bars->open == NULL) {
    free(bars);
    return NULL;
  }

  bars->high = malloc(sizeof(double) * numrows);
  if (bars->high == NULL) {
    free(bars);
    return NULL;
  }

  bars->low = malloc(sizeof(double) * numrows);
  if (bars->low == NULL) {
    free(bars);
    return NULL;
  }

  bars->close = malloc(sizeof(double) * numrows);
  if (bars->close == NULL) {
    free(bars);
    return NULL;
  }

  bars->adjclose = malloc(sizeof(double) * numrows);
  if (bars->adjclose == NULL) {
    free(bars);
    return NULL;
  }

  bars->volume = malloc(sizeof(int) * numrows);
  if (bars->volume == NULL) {
    free(bars);
    return NULL;
  }

  return bars;
}

void
ta_bars_destroy(ta_bars_t *bars)
{
  if (bars == NULL) {
    return;
  }

  free(bars->timestamp);
  free(bars->open);
  free(bars->high);
  free(bars->low);
  free(bars->close);
  free(bars->adjclose);
  free(bars->volume);
  free(bars);
}
