/*
 * yasm -f elf64 -p gas -o add_def.o add_att_x64.s
 * clang -c add_main.c -o add_main.o
 * clang -o add_main add_def.o add_main.o
 */

#include <stdio.h>

int add(int a, int b);

/*
 * This shows how to call a function implemented in Assembly from a C program.
 */
int main(void) {
    printf("%d\n", add(3, 5));

    return 0;
}
