#include <stdio.h>

#include "vector.h"

int main(void) {
	vector_t *vp;

        vp = vector_alloc(2);

	vector_free(&vp);

        return 0;
}
