#include <stdio.h>

#include "third_party/libxlsxwriter/include/xlsxwriter.h"

int
main(void)
{
  lxw_workbook  *workbook;
  lxw_worksheet *worksheet;

  workbook = workbook_new("dfp_export.xlsx");
  worksheet = workbook_add_worksheet(workbook, NULL);

  worksheet_write_string(worksheet, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet, 0, 2, "VL_CONTA", NULL);

  workbook_close(workbook);

  return 0;
}
