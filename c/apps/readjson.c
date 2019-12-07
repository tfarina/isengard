#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "third_party/cJSON/cJSON.h"

int main(int argc, char **argv) {
  FILE *f;
  long length;
  void *data;
  size_t bytes_read;
  cJSON *json;
  char *out;

  if (argc != 2) {
    fprintf(stderr, "usage: readjson <filename>\n");
    exit(EXIT_FAILURE);
  }

  f = fopen(argv[1], "rb");
  if (f == NULL || fseek(f, 0, SEEK_END) != 0) {
    goto out;
  }

  length = ftell(f);
  if (length < 0) {
    goto out;
  }

  data = malloc(length);
  if (data == NULL) {
    goto out;
  }
  rewind(f);

  bytes_read = fread(data, 1, length, f);
  if (ferror(f) != 0 || bytes_read != (size_t)length) {
    goto out;
  }

  json = cJSON_Parse((char*)data);
  if (!json) {
    fprintf(stderr, "cJSON_Parse() failed: %s\n", cJSON_GetErrorPtr());
    exit(EXIT_FAILURE);
  }

  out = cJSON_Print(json);
  cJSON_Delete(json);
  printf("%s\n", out);
  free(out);

out:
  if (f != NULL) {
    fclose(f);
  }
  free(data);

  return 0;
}
