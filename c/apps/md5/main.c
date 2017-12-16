#include <stdio.h>

#include "md5.h"

int main(int argc, char **argv) {
  FILE* infile;
  char* infilename;
  MD5_CTX ctx;
  int len;
  unsigned char buffer[1024];
  unsigned char digest[16];
  int i;

  if (argc <= 1) {
    return -1;
  }

  infilename = argv[1];

  if ((infile = fopen(infilename, "rb")) == NULL) {
    fprintf(stderr, "%s can't be opened.\n", infilename);
    return -1;
  }

  MD5_Init(&ctx);
  while ((len = fread(buffer, 1, sizeof(buffer), infile)) > 0) {
    MD5_Update(&ctx, buffer, (unsigned)len);
  }
  fclose(infile);
  MD5_Final(digest, &ctx);

  for (i = 0; i < sizeof(digest); i++) {
    fprintf(stdout, "%02x", digest[i]);
  }
  fprintf(stdout, "  %s\n", infilename);

  return 0;
}
