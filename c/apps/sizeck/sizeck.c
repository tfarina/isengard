#include <stdio.h>

int main(void) {
    printf("Remember: 1 byte = 8 bits\n\n");

    printf("int: %u bytes\n", sizeof(int));
    printf("float: %u bytes\n", sizeof(float));
    printf("double: %u bytes\n", sizeof(double));
    printf("char: %u byte\n", sizeof(char));

    printf("\n");

    printf("pointer to char: %d bytes\n", sizeof(char *));
    printf("pointer to void: %d bytes\n", sizeof(void *));
    return 0;
}
