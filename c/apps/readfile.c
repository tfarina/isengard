#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  FILE *f;
  long length;
  char *data;
  size_t bytes_read;

  if (argc != 2) {
    fprintf(stderr, "usage: readfile <filename>\n");
    exit(EXIT_FAILURE);
  }

  if ((f = fopen(argv[1], "rb")) == NULL) {
    fprintf(stderr, "error opening %s file\n", argv[1]);
    return -1;
  }

  if (fseek(f, 0, SEEK_END) == -1) {
    fprintf(stderr, "unable to fseek file %s\n", argv[1]);
    fclose(f);
    return -1;
  }

  if ((length = ftell(f)) == -1) {
    fprintf(stderr, "unable to ftell file %s\n", argv[1]);
    fclose(f);
    return -1;
  }

  if (fseek(f, 0, SEEK_SET) == -1) {
    fprintf(stderr, "unable to fseek file %s\n", argv[1]);
    fclose(f);
    return -1;
  }

  if ((data = malloc(length)) == NULL) {
    fprintf(stderr, "malloc failed (file too large?): %s\n", strerror(errno));
    fclose(f);
    return -1;
  }

  bytes_read = fread(data, 1, length, f);
  if (ferror(f) != 0 || bytes_read != (size_t)length) {
    fprintf(stderr, "fread failed\n");
    free(data);
    fclose(f);
    return -1;
  }

  fclose(f);

  fwrite(data, 1, length, stdout);

  free(data);

  return 0;
}
