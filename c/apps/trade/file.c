#include "file.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH_SEP '/'

char *read_file(const char *filename, size_t *len) {
  // open file for reading
  FILE *fh = fopen(filename, "r");
  if (fh == NULL) {
    perror("fopen()");
    return NULL;
  }
 
  int rc;
 
  // get file length
  rc = fseek(fh, 0, SEEK_END);
  if (rc < 0) {
    perror("fseek(END)");
    return NULL;
  }
  long l = ftell(fh);
  if (l < 0) {
    perror("ftell()");
    return NULL;
  }
  *len = l;
 
  // return file pointer to the beginning of input
  rc = fseek(fh, 0, SEEK_SET);
  if (rc < 0) {
    perror("fseek(SET)");
    return NULL;
  }
 
  // read in all file contents
  char *contents = malloc(*len);
  if (contents == NULL) {
    perror("malloc");
    return NULL;
  }
  size_t read = 0;
  while (read < *len) {
    size_t r = fread(contents + read, 1, *len - read, fh);
    if (r == 0) {
      if (ferror(fh)) {
	fputs("error reading input\n", stderr);
	free(contents);
	fclose(fh);
	return NULL;
      } else if (feof(fh)) {
	fprintf(stderr,
		"EOF encountered after %zu bytes (expected %zu)\n",
		read, *len);
	*len = read;
	break;
      }
    }
    read += r;
  }
 
  fclose(fh);
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

/* Code from msmtp:src/tools.c:get_filename */
char *make_file_path(const char *directory, const char *name)
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
