#include "ffileutils.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *f_build_filename(char *dir, char *file)
{
  char *path;
  size_t len;

  /* Add 2 bytes to the resulting length to have enough memory available
   * for the directory separator ('/') and the null byte ('\0').
   */
  len = strlen(dir) + strlen(file) + 2;

  path = malloc(sizeof(char) * len);
  if (path == NULL) {
    return NULL;
  }

  strcpy(path, dir);

  strcat(path, "/");

  strcat(path, file);

  return path;
}

/**
 * Reads the contents of |filename| and returns it.
 *
 * @return Returns NULL on failure.
 */
char *f_read_file(const char *filename, size_t *out_file_size)
{
  FILE *fp;
  int rv;
  long fsize;
  char *buf;
  size_t bytes_read;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "error opening %s file\n", filename);
    return NULL;
  }

  rv = fseek(fp, 0, SEEK_END);
  if (rv < 0) {
    fprintf(stderr, "unable to fseek file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  fsize = ftell(fp);
  if (fsize < 0) {
    fprintf(stderr, "unable to ftell file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  rv = fseek(fp, 0, SEEK_SET);
  if (rv < 0) {
    fprintf(stderr, "unable to fseek file %s\n", filename);
    fclose(fp);
    return NULL;
  }

  buf = malloc(sizeof(char) * fsize + 1);
  if (buf == NULL) {
    fprintf(stderr, "malloc failed (file too large?): %s\n", strerror(errno));
    fclose(fp);
    return NULL;
  }

  bytes_read = fread(buf, sizeof(char), fsize, fp);
  if (ferror(fp) != 0 || bytes_read != (size_t)fsize) {
    fprintf(stderr, "fread failed: %s\n", strerror(errno));
    free(buf);
    fclose(fp);
    return NULL;
  }

  buf[fsize] = '\0'; /* NUL-terminate the buffer, just to be safe. */
  fclose(fp);

  if (out_file_size) {
    *out_file_size = fsize;
  }

  return buf;
}

int f_write_file(const char *filename, const char *data, size_t size)
{
  int fd;
  size_t bytes_written_total = 0;

  fd = creat(filename, 0666);
  if (fd < 0) {
    return -1;
  }

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
