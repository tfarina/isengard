#include "ffileutils.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *build_filename(char *dirname, char *filename)
{
  char *path;
  size_t len;

  len = strlen(dirname) + /* '/' */ 1 + strlen(filename) + /* '\0' */ 1;

  path = (char *) malloc(len);

  sprintf(path, "%s/%s", dirname, filename);

  return path;
}

/**
 * Reads the contents of |filename| and returns it. |rlen| holds its
 * length.
 *
 * Returns NULL on failure.
 */
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

int f_write_file(const char *filename, const char *data, size_t size)
{
  int fd = creat(filename, 0666);
  if (fd < 0) {
    return -1;
  }

  ssize_t bytes_written_total = 0;
  for (ssize_t bytes_written_partial = 0; bytes_written_total < size;
       bytes_written_total += bytes_written_partial) {
    bytes_written_partial = write(fd, data + bytes_written_total,
                                      size - bytes_written_total);
    if (bytes_written_partial < 0) {
      return -1;
    }
  }

  if (close(fd) < 0) {
    return -1;
  }

  return size;
}
