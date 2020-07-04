#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ffileutils.h"

#include "third_party/cJSON/cJSON.h"

int main(int argc, char **argv) {
  char filename[] = "./menu.json";
  size_t fsize;
  char *file;
  cJSON *root;
  cJSON *j_menu, *j_obj;
  char *filestr;

  file = f_read_file(filename, &fsize);

  root = cJSON_Parse(file);
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

  return 0;
}
