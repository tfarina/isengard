#include "ffileutils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
 * Retrieves the size, in bytes, of the specified path.
 */
int f_file_size(char const *path)
{
  struct stat sb;
  int rc;

  rc = stat(path, &sb);
  if (rc < 0) {
    return -1;
  }

  return sb.st_size;
}

char *f_read_file(const char *filename, size_t *out_file_size)
{
  FILE *fp;
  long len;
  char *buf;
  size_t bytes_read;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "error opening %s file\n", filename);
    return NULL;
  }

  if (fseek(fp, 0, SEEK_END) == -1) {
    fprintf(stderr, "unable to fseek file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  len = ftell(fp);
  if (len == -1) {
    fprintf(stderr, "unable to ftell file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  if (fseek(fp, 0, SEEK_SET) == -1) {
    fprintf(stderr, "unable to fseek file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  buf = malloc(len);
  if (buf == NULL) {
    fprintf(stderr, "malloc failed (file too large?): %s\n", strerror(errno));
    fclose(fp);
    return NULL;
  }

  bytes_read = fread(buf, 1, len, fp);
  if (ferror(fp) != 0 || bytes_read != (size_t)len) {
    fprintf(stderr, "unable to fread file %s\n", filename);
    free(buf);
    fclose(fp);
    return NULL;
  }

  fclose(fp);

  if (out_file_size) {
    *out_file_size = len;
  }

  return buf;
}
