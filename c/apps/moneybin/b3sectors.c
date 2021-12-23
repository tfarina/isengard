#include "third_party/xlsxio/xlsxio_read.h"

#include <stdio.h>

int
main(void)
{
  xlsxioreader xlsxioread;
  xlsxioreadersheetlist sheetlist;
  XLSXIOCHAR const *sheetname;
  XLSXIOCHAR *value;
  xlsxioreadersheet sheet;

  xlsxioread = xlsxioread_open("b3/ClassifSetorial_30-06-2020.xlsx");
  if (!xlsxioread) {
    fprintf(stderr, "Error opening .xlsx file\n");
    return 1;
  }

  printf("Available sheets:\n");
  sheetlist = xlsxioread_sheetlist_open(xlsxioread);
  if (sheetlist != NULL) {
    while ((sheetname = xlsxioread_sheetlist_next(sheetlist)) != NULL) {
      printf(" - %s\n", sheetname);
    }
    xlsxioread_sheetlist_close(sheetlist);
  }

  printf("Contents of first sheet:\n");
  sheet = xlsxioread_sheet_open(xlsxioread, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
  while (xlsxioread_sheet_next_row(sheet)) {
    while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
      printf("%s\t", value);
      free(value);
    }
    printf("\n");
  }
  xlsxioread_sheet_close(sheet);

  xlsxioread_close(xlsxioread);

  return 0;
}
