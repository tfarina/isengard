#include "dounzip.h"

#include <stdio.h>
#include <stdlib.h>

#include "third_party/zlib/contrib/minizip/unzip.h"

#define WRITEBUFFERSIZE 8192

static int
do_list(unzFile uf)
{
  uLong i;
  unz_global_info64 gi;
  int err = 0;

  err = unzGetGlobalInfo64(uf, &gi);
  if (err != UNZ_OK) {
    fprintf(stderr, "error %d in zipfile in unzGetGlobalInfo64\n", err);
    return -1;
  }

  for (i = 0; i < gi.number_entry; i++) {
    char filename_inzip[256];
    unz_file_info64 file_info;

    err = unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
    if (err != UNZ_OK) {
      fprintf(stderr, "error %d with zipfile in unzGetCurrentFileInfo64\n", err);
      break;
    }

    printf("%s\n", filename_inzip);

    if ((i + 1) < gi.number_entry) {
      err = unzGoToNextFile(uf);
      if (err != UNZ_OK) {
        fprintf(stderr, "error %d with zipfile unzGoToNextFile\n", err);
	break;
      }
    }
  }

  return 0;
}

static int
do_extract_currentfile(unzFile uf)
{
  unz_file_info64 file_info;
  char filename_inzip[256];
  int err = UNZ_OK;
  void *buf;
  uInt  bufsize;
  char *filename_withoutpath;
  char *p;
  FILE *fout = NULL;

  err = unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
  if (err != UNZ_OK) {
    fprintf(stderr, "error %d with zipfile in unzGetCurrentFileInfo64\n", err);
    return err;
  }

  bufsize = WRITEBUFFERSIZE;
  buf = malloc(bufsize);
  if (buf == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    return UNZ_INTERNALERROR;
  }

  /*
   * This code will iterate through the characters of 'filename_inzip' until it reaches '\0'.
   * The string will be something like: 009512201912310401.dfp\000\000@\354\377\277
   * When it reaches the '\0', the only thing remaining will be 009512201912310401.dfp.
   */
  p = filename_withoutpath = filename_inzip;
  while ((*p) != '\0') {
    if (((*p) == '/') || ((*p) == '\\')) {
      filename_withoutpath = p + 1;
    }
    p++;
  }

  if ((*filename_withoutpath) == '\0') {

  } else {
    err = unzOpenCurrentFilePassword(uf, NULL);
    if (err != UNZ_OK) {
      fprintf(stderr, "error %d with zipfile in unzOpenCurrentFilePassword\n", err);
    }

    if (err == UNZ_OK) {
      fout = fopen(filename_inzip, "wb");
      if (fout == NULL) {
        fprintf(stderr, "error opening %s\n", filename_inzip);
      }
    }

    if (fout != NULL) {
#if 1
      printf("extracting: %s\n", filename_inzip);
#endif

      do {
	err = unzReadCurrentFile(uf, buf, bufsize);
        if (err < 0) {
          fprintf(stderr, "error %d with zipfile in unzReadCurrentFile\n", err);
          break;
        }
        if (err > 0) {
          if (fwrite(buf, err, 1, fout) != 1) {
	    fprintf(stderr, "error in writing extracted file\n");
            err = UNZ_ERRNO;
	    break;
	  }
	}
      } while (err > 0);

      if (fout) {
        fclose(fout);
      }
    } /* fout != NULL */

    if (err == UNZ_OK) {
      err = unzCloseCurrentFile(uf);
      if (err != UNZ_OK) {
	fprintf(stderr, "error %d with zipfile in unzCloseCurrentFile\n", err);
      }
    } else {
      unzCloseCurrentFile(uf); /* Don't lose the error. */
    }
  } /* else */

  free(buf);

  return err;
}

static int
do_extract(unzFile uf)
{
  uLong i;
  unz_global_info64 gi;
  int err;

  err = unzGetGlobalInfo64(uf, &gi);
  if (err != UNZ_OK) {
    fprintf(stderr, "error %d with zipfile unzGetGlobalInfo64\n", err);
    return -1;
  }

  for (i = 0; i < gi.number_entry; i++) {
    err = do_extract_currentfile(uf);
    if (err != UNZ_OK) {
      break;
    }

    if ((i + 1) < gi.number_entry) {
      err = unzGoToNextFile(uf);
      if (err != UNZ_OK) {
	fprintf(stderr, "error %d with zipfile in unzGoToNextFile\n", err);
	break;
      }
    }
  }

  return 0;
}

int
unzip(char const *zipfile, char const *exdir)
{
  unzFile uf = NULL;
  int retval = 0;

  uf = unzOpen64(zipfile);
  if (uf == NULL) {
    fprintf(stderr, "Failed to open %s\n", zipfile);
    return -1;
  }

#if 0
  retval = do_list(uf);
#endif

  retval = do_extract(uf);

  unzClose(uf);

  return 0;
}
