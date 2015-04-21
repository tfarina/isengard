#include <stdio.h>

#include "md5.h"

int main(int argc, char** argv) {
  if (argc <= 1) {
    return -1;
  }

  char* infilename = argv[1];
  FILE* infile;

  if ((infile = fopen(infilename, "rb")) == NULL) {
    fprintf(stderr, "%s can't be opened.\n", infilename);
    return -1;
  }

  int len;
  unsigned char buffer[1024];

  MD5_CTX ctx;
  MD5_Init(&ctx);
  while ((len = fread(buffer, 1, sizeof(buffer), infile)) > 0) {
    MD5_Update(&ctx, buffer, (unsigned)len);
  }

  fclose(infile);

  unsigned char digest[16];
  MD5_Final(digest, &ctx);

  for (int i = 0; i < sizeof(digest); i++) {
    fprintf(stdout, "%02x", digest[i]);
  }
  fprintf(stdout, "  %s", infilename);
  printf("\n");
  return 0;
}
