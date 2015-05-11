/* yasm -f elf64 -p gas add_intel_x64.s -o add_def.o
 * clang -c add_main.c -o add_main.o
 * clang -o add_main add_def.o add_main.o
 */

#include <stdio.h>

int add(int a, int b);

int main(void) {
    printf("%d\n", add(3, 5));

    return 0;
}
