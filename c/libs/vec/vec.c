#include "vec.h"

#include <stdlib.h>

#include "os.h"

void vec_grow(vec_t *v, int n);

void vec_init(vec_t *v)
{
	v->strs = NULL;
	v->len = 0;
	v->cap = 0;
}

void vec_reset(vec_t *v)
{
	int i;

        for (i = 0; i < v->len; i++) {
                free(v->strs[i]);
		v->strs[i] = NULL;
	}
	v->len = 0;
}

void vec_free(vec_t *v)
{
	vec_reset(v);
	free(v->strs);
	vec_init(v);
}

// vgrow ensures that the vector has room for at least
// n more entries between len and cap.
void vec_grow(vec_t *v, int n)
{
	int want;

        want = v->len + n;
        if(want > v->cap) {
                v->cap = 2 * want;
                if(v->cap < 64)
			v->cap = 64;
                v->strs = xrealloc(v->strs, v->cap * sizeof v->strs[0]);
        }
}

void vec_add(vec_t *v, char *str)
{
	vec_grow(v, 1);
	if(str != NULL)
		str = xstrdup(str);
	v->strs[v->len++] = str;
}

void vec_copy(vec_t *dst, char **src, int srclen)
{
	int i;

	// use vadd, to make copies of strings
        for (i = 0; i < srclen; i++)
                vec_add(dst, src[i]);
}
