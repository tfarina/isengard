#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

int main(int argc, char **argv) {
  FILE *f;
  long length;
  void *data;
  size_t bytes_read;
  cJSON *root;
  cJSON *menu;
  char *filestr;

  f = fopen("./menu.json", "rb");
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

  root = cJSON_Parse((char*)data);
  if (!root) {
    fprintf(stderr, "cJSON_Parse() failed: %s\n", cJSON_GetErrorPtr());
    exit(EXIT_FAILURE);
  }

  menu= cJSON_GetObjectItem(root, "menu");
  filestr = cJSON_GetArrayItem(menu, 1)->valuestring;
  printf("menu value: %s\n", filestr);

out:
  if (f != NULL) {
    fclose(f);
  }
  free(data);

  return 0;
}
