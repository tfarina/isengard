#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *f;
  long length;
  void *data;
  size_t bytes_read;

  if (argc != 2) {
    fprintf(stderr, "usage: readfile <filename>\n");
    exit(EXIT_FAILURE);
  }

  f = fopen(argv[1], "rb");
  if (f == NULL) {
    goto out;
  }

  if (fseek(f, 0, SEEK_END) == -1) {
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

  fwrite(data, 1, length, stdout);

out:
  if (f != NULL) {
    fclose(f);
  }
  free(data);

  return 0;
}
