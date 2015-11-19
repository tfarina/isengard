#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static char *read_file(const char *filename, size_t *rlen)
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

int main(int argc, char **argv) {
  size_t len;
  char *buf;
  const char *c;

  if (argc != 2) {
    fprintf(stderr, "usage: lexer <filename>\n");
    exit(EXIT_FAILURE);
  }

  buf = read_file(argv[1], &len);

  for (c = buf; *c; ++c) {
    switch (*c) {
      case '{':
        printf("Token::BRACE_L\n");
        break;
      case '}':
        printf("Token::BRACE_R\n");
        break;
    }
  }

  free(buf);

  return 0;
}
