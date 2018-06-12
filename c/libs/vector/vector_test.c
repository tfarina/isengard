#include <stdio.h>

#include "vector.h"

typedef struct {
        int x;
        int y;
} point_t;

int main(void) {
        point_t p, p2, p3;
	vector_t *vp;
        int i;

        p.x = 2;
        p.y = 3;

        p2.x = 5;
        p2.y = 8;

        p3.x = 13;
        p3.y = 21;

        vp = vector_alloc(2);

        printf("%d\n", vector_length(vp));

        vector_append(vp, &p, sizeof(point_t));
        vector_append(vp, &p2, sizeof(point_t));
        vector_append(vp, &p3, sizeof(point_t));

        printf("%d\n", vector_length(vp));

        for (i = 0; i < vector_length(vp); i++) {
                point_t *point = vector_get(vp, i);

                printf("x: %d, y: %d\n", point->x, point->y);
        }

	vector_free(&vp);

        printf("%d\n", vector_length(vp));

        return 0;
}
