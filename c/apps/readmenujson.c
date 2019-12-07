#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "third_party/cJSON/cJSON.h"

int main(int argc, char **argv) {
  FILE *f;
  long length;
  void *data;
  size_t bytes_read;
  cJSON *root;
  cJSON *j_menu, *j_obj;
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

  j_menu = cJSON_GetObjectItem(root, "menu");
  if (!j_menu || !cjson_is_object(j_menu)) {
    fprintf(stderr, "no proper menu object returned\n");
    exit(EXIT_FAILURE);
  }

  j_obj = cJSON_GetObjectItem(j_menu, "value");
  if (!j_obj || !cjson_is_string(j_obj)) {
    fprintf(stderr, "menu object has no 'value'\n");
    exit(EXIT_FAILURE);
  }

  filestr = j_obj->valuestring;
  printf("menu value: %s\n", filestr);

  cJSON_Delete(root);

out:
  if (f != NULL) {
    fclose(f);
  }
  free(data);

  return 0;
}
