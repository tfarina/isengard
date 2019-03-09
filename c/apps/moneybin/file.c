#include "file.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *read_file(const char *filename, size_t *len) {
  FILE *fp;
  int rc;
  long bufsize;
  char *contents;
  size_t bytes_read;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("fopen()");
    return NULL;
  }
 
  /* Go to the end of the file. */
  rc = fseek(fp, 0, SEEK_END);
  if (rc < 0) {
    perror("fseek(END)");
    return NULL;
  }

  /* To get the size of the file. */
  bufsize = ftell(fp);
  if (bufsize < 0) {
    perror("ftell()");
    return NULL;
  }
 
  /* Go back to the start of the file. */
  rc = fseek(fp, 0, SEEK_SET);
  if (rc < 0) {
    perror("fseek(SET)");
    return NULL;
  }
 
  /* Allocate buffer memory for that file size.
   * The +1 is to add room for adding the NULL-terminator at the end.
   */
  contents = malloc(sizeof(char) * bufsize + 1);
  if (contents == NULL) {
    perror("malloc");
    return NULL;
  }

  bytes_read = fread(contents, sizeof(char), bufsize, fp);

  if ((bytes_read == 0 && ferror(fp)) || feof(fp)) {
    fprintf(stderr, "Error reading file \"%s\"\n", filename);
    free(contents);
    fclose(fp);
    return NULL;
  }
 
  fclose(fp);

  contents[++bytes_read] = '\0'; /* NULL-terminate the string, just to be safe. */

  if (len) {
    *len = bufsize;
  }

  return contents;
}

int write_file(const char *filename, const char *data, size_t size)
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
