#include <ctype.h>
#include <stdio.h>

/* From Baidu Tera.
 *
 * Run with the following command line:
 *
 * LC_ALL=C ./ascii_table
 *
 */
int main(int argc, char **argv) {
  int i;

  for (i = 0; i < 128; ++i) {
     printf("            /* 0x%02x(%c) */ ", i, isgraph(i) ? i : ' ');
     if (isblank(i)) printf("kBlank | ");
     if (isspace(i)) printf("kSpace | ");
     if (isupper(i)) printf("kUpper | ");
     if (islower(i)) printf("kLower | ");
     if (isdigit(i)) printf("kDigit | ");
     if (isxdigit(i)) printf("kHexDigit | ");
     if (ispunct(i)) printf("kPunct | ");
     if (iscntrl(i)) printf("kControl | ");
     if (isgraph(i)) printf("kGraph | ");
     if (isprint(i)) printf("kPrint | ");
     printf("0,\n");
  }

  return 0;
}
