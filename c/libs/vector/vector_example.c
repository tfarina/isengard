#include <stdio.h>

#include "vector.h"

typedef struct {
        int x;
        int y;
} point_t;

int main(void) {
        point_t p, p2, p3;
	vector_t *vp;
	size_t vsize;
        size_t i;

        p.x = 2;
        p.y = 3;

        p2.x = 5;
        p2.y = 8;

        p3.x = 13;
        p3.y = 21;

        vp = vector_create(sizeof(point_t), 2);

        printf("%d\n", vector_size(vp));

        vector_push_back(vp, &p);
        vector_push_back(vp, &p2);
        vector_push_back(vp, &p3);

	vsize = vector_size(vp);
        printf("%d\n", vsize);

        for (i = 0; i < vsize; i++) {
                point_t *point = vector_at(vp, i);

                printf("x: %d, y: %d\n", point->x, point->y);
        }

	vector_destroy(vp);

        printf("%d\n", vector_size(vp));

        return 0;
}
