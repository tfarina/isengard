#include "ffileutils.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH_SEP '/'

/* Code from msmtp:src/tools.c:get_filename */
char *f_build_filename(const char *directory, const char *name)
{
  char *path;
  size_t dirlen;

  dirlen = strlen(directory);
  path = malloc((dirlen + strlen(name) + 2) * sizeof(char));
  strcpy(path, directory);
  if (dirlen == 0 || path[dirlen - 1] != PATH_SEP) {
    path[dirlen++] = PATH_SEP;
  }
  strcpy(path + dirlen, name);
  return path;
}

int f_read_file(const char *filename, char **out_contents, size_t *out_file_size)
{
  FILE *fp;
  int rc;
  long bufsize;
  char *contents;
  size_t bytes_read;

  /* Open the file for reading. */
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("fopen()");
    return -1;
  }

  /* Go to the end on the stream. */
  rc = fseek(fp, 0, SEEK_END);
  if (rc < 0) {
    perror("fseek(END)");
    return -1;
  }

  /* And get the file size. */
  bufsize = ftell(fp);
  if (bufsize < 0) {
    perror("ftell()");
    return -1;
  }

  /* Return the file's pointer to the beginning of the stream. */
  rc = fseek(fp, 0, SEEK_SET);
  if (rc < 0) {
    perror("fseek(SET)");
    return -1;
  }

  /*
   * Allocate enough memory for the new buffer.
   *
   * The +1 is to add room for adding the NULL-terminator at the end.
   */
  contents = malloc(sizeof(char) * bufsize + 1);
  if (contents == NULL) {
    perror("malloc");
    fclose(fp);
    return -1;
  }

  bytes_read = fread(contents, sizeof(char), bufsize, fp);
  if ((bytes_read == 0 && ferror(fp)) || feof(fp)) {
    fprintf(stderr, "Error reading file \"%s\"\n", filename);
    free(contents);
    fclose(fp);
    return -1;
  }

  fclose(fp);

  contents[++bytes_read] = '\0'; /* NULL-terminate the string, just to be safe. */

  if (out_file_size) {
    *out_file_size = bufsize;
  }

  *out_contents = contents;

  return 0;
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
