#include <stdio.h>

/*                  Power of 2 values      */

/*             Hex         | Binary                                  | Decimal       */
#define FLAG01 0x01       /*                                       1 | 1             */
#define FLAG02 0x02       /*                                      10 | 2             */
#define FLAG03 0x04       /*                                     100 | 4             */
#define FLAG04 0x08       /*                                    1000 | 8             */
#define FLAG05 0x10       /*                                  1 0000 | 16            */
#define FLAG06 0x20       /*                                 10 0000 | 32            */
#define FLAG07 0x40       /*                                100 0000 | 64            */
#define FLAG08 0x80       /*                               1000 0000 | 128           */
#define FLAG09 0x100      /*                             1 0000 0000 | 256           */
#define FLAG10 0x200      /*                            10 0000 0000 | 512           */
#define FLAG11 0x400      /*                           100 0000 0000 | 1,024         */
#define FLAG12 0x800      /*                          1000 0000 0000 | 2,048         */
#define FLAG13 0x1000     /*                        1 0000 0000 0000 | 4,096         */
#define FLAG14 0x2000     /*                       10 0000 0000 0000 | 8,182         */
#define FLAG15 0x4000     /*                      100 0000 0000 0000 | 16,384        */
#define FLAG16 0x8000     /*                     1000 0000 0000 0000 | 32,768        */
#define FLAG17 0x10000    /*                   1 0000 0000 0000 0000 | 65,536        */
#define FLAG18 0x20000    /*                  10 0000 0000 0000 0000 | 131,072       */
#define FLAG19 0x40000    /*                 100 0000 0000 0000 0000 | 262,144       */
#define FLAG20 0x80000    /*                1000 0000 0000 0000 0000 | 524,288       */
#define FLAG21 0x100000   /*              1 0000 0000 0000 0000 0000 | 1,048,576     */
#define FLAG22 0x200000   /*             10 0000 0000 0000 0000 0000 | 2,097,304     */
#define FLAG23 0x400000   /*            100 0000 0000 0000 0000 0000 | 4,194,304     */
#define FLAG24 0x800000   /*           1000 0000 0000 0000 0000 0000 | 8,388,608     */
#define FLAG25 0x1000000  /*         1 0000 0000 0000 0000 0000 0000 | 16,777,216    */
#define FLAG26 0x2000000  /*        10 0000 0000 0000 0000 0000 0000 | 33,554,432    */
#define FLAG27 0x4000000  /*       100 0000 0000 0000 0000 0000 0000 | 67,108,864    */
#define FLAG28 0x8000000  /*      1000 0000 0000 0000 0000 0000 0000 | 134,217,728   */
#define FLAG29 0x10000000 /*    1 0000 0000 0000 0000 0000 0000 0000 | 268,435,456   */
#define FLAG30 0x20000000 /*   10 0000 0000 0000 0000 0000 0000 0000 | 536,870,912   */
#define FLAG31 0x40000000 /*  100 0000 0000 0000 0000 0000 0000 0000 | 1,073,741,824 */
#define FLAG32 0x80000000 /* 1000 0000 0000 0000 0000 0000 0000 0000 | 2,147,483,648 */

#define BITMASK(index) (1 << (index))

int main(void) {

  /* This will print the decimal values from the hexadecimal constants
   */
  printf("%u\n", FLAG01);
  printf("%u\n", FLAG02);
  printf("%u\n", FLAG03);
  printf("%u\n", FLAG04);
  printf("%u\n", FLAG05);
  printf("%u\n", FLAG06);
  printf("%u\n", FLAG07);
  printf("%u\n", FLAG08);
  printf("%u\n", FLAG09);
  printf("%u\n", FLAG10);
  printf("%u\n", FLAG11);
  printf("%u\n", FLAG12);
  printf("%u\n", FLAG13);
  printf("%u\n", FLAG14);
  printf("%u\n", FLAG15);
  printf("%u\n", FLAG16);
  printf("%u\n", FLAG17);
  printf("%u\n", FLAG18);
  printf("%u\n", FLAG19);
  printf("%u\n", FLAG20);
  printf("%u\n", FLAG21);
  printf("%u\n", FLAG22);
  printf("%u\n", FLAG23);
  printf("%u\n", FLAG24);
  printf("%u\n", FLAG25);
  printf("%u\n", FLAG26);
  printf("%u\n", FLAG27);
  printf("%u\n", FLAG28);
  printf("%u\n", FLAG29);
  printf("%u\n", FLAG30);
  printf("%u\n", FLAG31);
  printf("%u\n", FLAG32);

  /*
   * left-shift operator (<<)
   *
   * What does it do? It tells the compiler to move the bit on the most right
   * to the left by n positions.
   *
   * If you start with 0000 0001, it creates a 'power of 2' effect.
   *
   * In other words, left shifting (<<) an integer "x" with an integer "y"
   * (x << y) is equivalent to multiplying x by 2^y (2 raise to power y).
   */
  printf("%u\n", BITMASK(0)); /* 0000 0001 | 0x01 | 1   */
  printf("%u\n", BITMASK(1)); /* 0000 0010 | 0x02 | 2   */
  printf("%u\n", BITMASK(2)); /* 0000 0100 | 0x04 | 4   */
  printf("%u\n", BITMASK(3)); /* 0000 1000 | 0x08 | 8   */
  printf("%u\n", BITMASK(4)); /* 0001 0000 | 0x10 | 16  */
  printf("%u\n", BITMASK(5)); /* 0010 0000 | 0x20 | 32  */
  printf("%u\n", BITMASK(6)); /* 0100 0000 | 0x40 | 64  */
  printf("%u\n", BITMASK(7)); /* 1000 0000 | 0x80 | 128 */

  return 0;
}
