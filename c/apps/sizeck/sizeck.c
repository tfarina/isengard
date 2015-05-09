#include <stdio.h>

int main(void) {
    printf("Remember: 1 byte = 8 bits\n\n");

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
