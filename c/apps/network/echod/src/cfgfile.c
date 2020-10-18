#include "cfgfile.h"

#include <stdio.h>
#include <stdlib.h>

void read_config_file(char const *cfgfile) {
  FILE *fp;

  fp = fopen(cfgfile, "r");
  if (fp == NULL) {
    return;
  }
}
