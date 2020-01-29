#include <stdio.h>

int main(void) {
    printf("Data Type Sizes:\n\n");

    printf("Remember: 1 byte = 8 bits\n");
    printf("With 8 bits (00000000, 00000001, etc) it's possible to "
           "represent 256 (0 to 255) (2^8) values.\n");
    printf("This is possible because one bit can only be either 0 or 1.\n\n");

    printf("char: %zu byte\n", sizeof(char));
    printf("short: %zu bytes\n", sizeof(short));
    printf("int: %zu bytes\n", sizeof(int));
    printf("long: %zu bytes\n", sizeof(long));
    printf("float: %zu bytes\n", sizeof(float));
    printf("double: %zu bytes\n", sizeof(double));

    printf("\n");

    printf("pointer to char: %zu bytes\n", sizeof(char *));
    printf("pointer to int: %zu bytes\n", sizeof(int *));
    printf("pointer to void: %zu bytes\n", sizeof(void *));

    return 0;
}
