#include <stdio.h>

#include "vector.h"

int main(void) {
	vector_t *vp;

        vp = vector_alloc(2);

        printf("%d\n", vector_length(vp));

	vector_free(&vp);

        return 0;
}
