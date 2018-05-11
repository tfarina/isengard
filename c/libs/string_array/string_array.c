#include "string_array.h"

#include <stdlib.h>

#include "xalloc.h"

void string_array_grow(string_array_t *s, int n);

void string_array_init(string_array_t *s)
{
	s->strs = NULL;
	s->len = 0;
	s->cap = 0;
}

void string_array_reset(string_array_t *s)
{
	int i;

        for (i = 0; i < s->len; i++) {
                free(s->strs[i]);
		s->strs[i] = NULL;
	}
	s->len = 0;
}

void string_array_free(string_array_t *s)
{
	string_array_reset(s);
	free(s->strs);
	string_array_init(s);
}

// string_array_grow ensures that the list has room for at least
// n more entries between len and cap.
void string_array_grow(string_array_t *s, int n)
{
	int want;

        want = s->len + n;
        if(want > s->cap) {
                s->cap = 2 * want;
                if (s->cap < 64)
			s->cap = 64;
                s->strs = xrealloc(s->strs, s->cap * sizeof s->strs[0]);
        }
}

void string_array_add(string_array_t *s, char *str)
{
	string_array_grow(s, 1);
	if(str != NULL)
		str = xstrdup(str);
	s->strs[s->len++] = str;
}

void string_array_copy(string_array_t *dst, char **src, int srclen)
{
	int i;

	/* Use string_array_add(), to make copies of strings. */
        for (i = 0; i < srclen; i++)
                string_array_add(dst, src[i]);
}
