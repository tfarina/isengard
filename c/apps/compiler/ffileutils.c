#include "ffileutils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
 * Returne the size, in bytes, of path.
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

char *f_read_file(const char *filename, size_t *rlen)
{
  FILE *fp;
  long len;
  char *buf;
  size_t bytes_read;

  if ((fp = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "error opening %s file\n", filename);
    return NULL;
  }

  if (fseek(fp, 0, SEEK_END) == -1) {
    fprintf(stderr, "unable to fseek file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  if ((len = ftell(fp)) == -1) {
    fprintf(stderr, "unable to ftell file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  if (fseek(fp, 0, SEEK_SET) == -1) {
    fprintf(stderr, "unable to fseek file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  if ((buf = malloc(len)) == NULL) {
    fprintf(stderr, "malloc failed (file too large?): %s\n", strerror(errno));
    fclose(fp);
    return NULL;
  }

  bytes_read = fread(buf, 1, len, fp);
  if (ferror(fp) != 0 || bytes_read != (size_t)len) {
    fprintf(stderr, "fread failed\n");
    free(buf);
    fclose(fp);
    return NULL;
  }

  fclose(fp);

  *rlen = len;
  return buf;
}
