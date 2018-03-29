#include <stdio.h>

int main(void) {
    printf("Sizes of Data Types:\n\n");

    printf("Remember: 1 byte = 8 bits\n");
    printf("With 8 bits (00000000, 00000001, etc) it's possible to "
           "represent 256 (0 to 255) (2^8) values.\n");
    printf("This is possible because one bit can only be either 0 or 1.\n\n");

    printf("char: %lu byte\n", sizeof(char));
    printf("short: %lu bytes\n", sizeof(short));
    printf("int: %lu bytes\n", sizeof(int));
    printf("long: %lu bytes\n", sizeof(long));
    printf("float: %lu bytes\n", sizeof(float));
    printf("double: %lu bytes\n", sizeof(double));

    printf("\n");

    printf("pointer to char: %lu bytes\n", sizeof(char *));
    printf("pointer to int: %lu bytes\n", sizeof(int *));
    printf("pointer to void: %lu bytes\n", sizeof(void *));

    return 0;
}
