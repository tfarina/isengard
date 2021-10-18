#include <stdio.h>

#include "third_party/zip/zip.h"

int
main(int argc, char **argv)
{
  char *filename;
  struct zip_t *zip;
  int nb_entries; /* total */
  int i;

  if (argc < 2) {
    fputs("usage: readdfp <filename>\n", stderr);
    fputc('\n', stderr);
    fputs("Example:\n", stderr);
    fputc('\n', stderr);
    fputs("   readdfp 009512201912310401.zip\n", stderr);
    return 1;
  }

  filename = argv[1];

  zip = zip_open(filename, 0, 'r');
  nb_entries = zip_entries_total(zip);

  for (i = 0; i < nb_entries; ++i) {
    zip_entry_openbyindex(zip, i);
    {
      const char *name = zip_entry_name(zip);
      int isdir = zip_entry_isdir(zip);
      unsigned long long size = zip_entry_size(zip);
      unsigned int crc32 = zip_entry_crc32(zip);
    }
    zip_entry_close(zip);
  }

  zip_close(zip);

  return 0;
}
