#include <stdio.h>

#include "third_party/libxlsxwriter/include/xlsxwriter.h"

int
main(void)
{
  lxw_workbook  *workbook;
  lxw_worksheet *worksheet;

  workbook = workbook_new("hello_excel.xlsx");
  worksheet = workbook_add_worksheet(workbook, NULL);

  worksheet_write_string(worksheet, 0, 0, "Hello Excel", NULL);

  workbook_close(workbook);

  return 0;
}
