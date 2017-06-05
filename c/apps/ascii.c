#include <stdio.h>

/* This works for characters from 0-127, because they go
 * from 00000000 to 01111111.
 * 
 * Since 0x80 is 128 in decimal and 10000000 in binary,
 * if the character 'c' is in the range 00000000-01111111,
 * the result of the operation (c & 10000000) has to be
 * zero, otherwise the result will be something other than
 * zero and the test (== 0) will be false.
 */
static int ascii_is_valid(char c)
{
  return (c & 0x80) == 0;
}

int main(int argc, char **argv)
{
  if (ascii_is_valid('a') == 1) {
    printf("'a' is a valid ascii character\n");
  }

  if (ascii_is_valid(0x80) == 1) {
    printf("This is in the extended ascii codes range (128-255)\n");
  }

  return 0;
}
