#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "ffileutils.h"

/**
 * https://github.com/leahneukirchen/mblaze/blob/fc4c696e3df800dec5aa4fd6c7745b4e38dbe40a/blaze822.c#L454
 */
static char *
readfile(char const *filename, size_t *out_file_size)
{
  int fd;
  struct stat st;
  off_t fsize;
  char *buf = NULL;
  ssize_t num_read, total_read = 0;

  fd = open(filename, O_RDONLY);
  if (fd < 0) {
    return NULL;
  }

  if (fstat(fd, &st) < 0) {
    goto error;
  }

  fsize = st.st_size;

  buf = malloc(sizeof(char) * fsize + 1);
  if (buf == NULL) {
    goto error;
  }

  do {
    num_read = read(fd, buf + total_read, fsize - total_read);
    if (num_read < 0) {
      if (errno == EINTR) {
	continue;
      } else {
	perror("read");
	goto error;
      }
    }

    total_read += num_read;
  } while (total_read < fsize && num_read > 0);

  close(fd);

  buf[total_read] = 0;

  if (out_file_size) {
    *out_file_size = fsize;
  }

  return buf;

error:
  close(fd);
  free(buf);
  return NULL;
}

int main(int argc, char **argv) {
  size_t len;
  char *buf;

  if (argc != 2) {
    fprintf(stderr, "usage: readfile <filename>\n");
    return 1;
  }

  buf = f_read_file(argv[1], &len);
  if (!buf) {
    free(buf);
    return 1;
  }

  fwrite(buf, 1, len, stdout);

  free(buf);

  buf = readfile(argv[1], &len);
  if (!buf) {
    free(buf);
    return 1;
  }

  fwrite(buf, 1, len, stdout);

  free(buf);

  return 0;
}
