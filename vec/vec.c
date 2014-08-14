#include "vec.h"

#include "unix.h"

void vec_grow(Vec *v, int n);

void vec_init(Vec *v)
{
	v->p = nil;
	v->len = 0;
	v->cap = 0;
}

void vec_reset(Vec *v)
{
	int i;

        for (i = 0; i < v->len; i++) {
                xfree(v->p[i]);
		v->p[i] = nil;
	}
	v->len = 0;
}

void vec_free(Vec *v)
{
	vec_reset(v);
	xfree(v->p);
	vec_init(v);
}

// vgrow ensures that the vector has room for at least
// n more entries between len and cap.
void vec_grow(Vec *v, int n)
{
	int want;

        want = v->len + n;
        if(want > v->cap) {
                v->cap = 2 * want;
                if(v->cap < 64)
			v->cap = 64;
                v->p = xrealloc(v->p, v->cap * sizeof v->p[0]);
        }
}

void vec_add(Vec *v, char *p)
{
	vec_grow(v, 1);
	if(p != nil)
		p = xstrdup(p);
	v->p[v->len++] = p;
}

void vec_copy(Vec *dst, char **src, int srclen)
{
	int i;

	// use vadd, to make copies of strings
        for (i = 0; i < srclen; i++)
                vec_add(dst, src[i]);
}
