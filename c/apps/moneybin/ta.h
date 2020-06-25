#ifndef TA_H_
#define TA_H_

enum {
  TA_SUCCESS = 0,    /* No error */
  TA_FAILURE = -1
};

typedef struct ta_bars_s {
  long unsigned numrows;
  double *open;
  double *high;
  double *low;
  double *close;
  double *adjclose;
  int *volume;
} ta_bars_t;

int read_csv(char const *filename, ta_bars_t **outbars);

#endif  /* TA_H_ */
