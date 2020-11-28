#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
#include "dounzip.h"
#include "download.h"

#define MAXURLLEN 256

static int verbose = 1;

int main(int argc, char **argv)
{
  int numseqdoc;
  char url[MAXURLLEN];
  char *destfile;

  if (argc < 2) {
    fputs("usage: fetchdfp numseqdoc\n", stderr);
    return 1;
  }

  numseqdoc = atoi(argv[1]);

  memset(url, 0, MAXURLLEN);
  snprintf(url, sizeof(url), "https://www.rad.cvm.gov.br/enetconsulta/frmDownloadDocumento.aspx?"
	   "CodigoInstituicao=2&NumeroSequencialDocumento=%d", numseqdoc);

  destfile = "dfp.zip";

  download_file(url, destfile);

  unzip(destfile, ".");

  return 0;
}
