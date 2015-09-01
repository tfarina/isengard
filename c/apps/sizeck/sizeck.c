#include <stdio.h>

int main(void) {
    printf("Remember: 1 byte = 8 bits\n");
    printf("and with 8 bits it is possible to represent 256 (2^8) values\n");
    printf("this is also possible because a bit can only be either 0 or 1\n\n");

    printf("char: %lu byte\n", sizeof(char));
    printf("short: %lu bytes\n", sizeof(short));
    printf("int: %lu bytes\n", sizeof(int));
    printf("float: %lu bytes\n", sizeof(float));
    printf("double: %lu bytes\n", sizeof(double));

    printf("\n");

    printf("pointer to char: %lu bytes\n", sizeof(char *));
    printf("pointer to void: %lu bytes\n", sizeof(void *));
    return 0;
}
