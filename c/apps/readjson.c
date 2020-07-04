#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ffileutils.h"

#include "third_party/cJSON/cJSON.h"

int main(int argc, char **argv) {
  char *filename = NULL;
  size_t fsize;
  char *file;
  cJSON *json;
  char *out;

  if (argc != 2) {
    fprintf(stderr, "usage: readjson <filename>\n");
    exit(EXIT_FAILURE);
  }

  filename = argv[1];

  file = f_read_file(filename, &fsize);

  json = cJSON_Parse(file);
  if (!json) {
    fprintf(stderr, "cJSON_Parse() failed: %s\n", cJSON_GetErrorPtr());
    exit(EXIT_FAILURE);
  }

  out = cJSON_Print(json);
  cJSON_Delete(json);
  printf("%s\n", out);
  free(out);

  return 0;
}
