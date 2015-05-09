// yasm -f elf64 -p gas add2.asm -o add_def.o
// clang -c add_main.c -o add_main.o
// clang add_main.o add_def.o -o add_main

#include <stdio.h>

int add(int a, int b);

int main(void) {
    printf("%d\n", add(3, 5));

    return 0;
}
