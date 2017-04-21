#include "string_list.h"

#include <stdlib.h>

#include "os.h"

void string_list_grow(string_list_t *s, int n);

void string_list_init(string_list_t *s)
{
	s->strs = NULL;
	s->len = 0;
	s->cap = 0;
}

void string_list_reset(string_list_t *s)
{
	int i;

        for (i = 0; i < s->len; i++) {
                free(s->strs[i]);
		s->strs[i] = NULL;
	}
	s->len = 0;
}

void string_list_free(string_list_t *s)
{
	string_list_reset(s);
	free(s->strs);
	string_list_init(s);
}

// string_list_grow ensures that the list has room for at least
// n more entries between len and cap.
void string_list_grow(string_list_t *s, int n)
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

void string_list_add(string_list_t *s, char *str)
{
	string_list_grow(s, 1);
	if(str != NULL)
		str = xstrdup(str);
	s->strs[s->len++] = str;
}

void string_list_copy(string_list_t *dst, char **src, int srclen)
{
	int i;

	/* Use string_list_ad(), to make copies of strings. */
        for (i = 0; i < srclen; i++)
                string_list_add(dst, src[i]);
}
