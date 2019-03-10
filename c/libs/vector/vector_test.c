#include <stdio.h>

#include "vector.h"

typedef struct {
        int x;
        int y;
} point_t;

int main(void) {
        point_t p, p2, p3;
	vector_t *vp;
	size_t vp_len;
        size_t i;

        p.x = 2;
        p.y = 3;

        p2.x = 5;
        p2.y = 8;

        p3.x = 13;
        p3.y = 21;

        vp = vector_alloc(2);

        printf("%d\n", vector_size(vp));

        vector_append(vp, &p);
        vector_append(vp, &p2);
        vector_append(vp, &p3);

	vp_len = vector_size(vp);
        printf("%d\n", vp_len);

        for (i = 0; i < vp_len; i++) {
                point_t *point = vector_get(vp, i);

                printf("x: %d, y: %d\n", point->x, point->y);
        }

	vector_free(&vp);

        printf("%d\n", vector_size(vp));

        return 0;
}
