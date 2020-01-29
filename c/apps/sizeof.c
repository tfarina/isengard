#include <stdio.h>

int main(void) {
    printf("Data Type Sizes:\n\n");

    printf("Remember: 1 byte = 8 bits\n");
    printf("With 8 bits (00000000, 00000001, etc) it's possible to "
           "represent 256 (0 to 255) (2^8) values.\n");
    printf("This is possible because one bit can only be either 0 or 1.\n\n");

    printf("%18s | %4zu byte\n", "char", sizeof(char));
    printf("%18s | %4zu bytes\n", "short", sizeof(short));
    printf("%18s | %4zu bytes\n", "int", sizeof(int));
    printf("%18s | %4zu bytes\n", "long", sizeof(long));
    printf("%18s | %4zu bytes\n", "float", sizeof(float));
    printf("%18s | %4zu bytes\n", "double", sizeof(double));

    printf("\n");

    printf("%18s | %4zu bytes\n", "pointer to char", sizeof(char *));
    printf("%18s | %4zu bytes\n", "pointer to int", sizeof(int *));
    printf("%18s | %4zu bytes\n", "pointer to void", sizeof(void *));

    return 0;
}
