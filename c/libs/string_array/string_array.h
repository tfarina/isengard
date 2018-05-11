#ifndef STRING_ARRAY_H_
#define STRING_ARRAY_H_

/*
 * string_array_t is like Go's []string.
 */
typedef struct {
	char **strs;
	int len;
	int cap;
} string_array_t;

/*
 * string_array_init prepares an uninitialized list for use.
 */
void string_array_init(string_array_t *s);

/*
 * string_array_reset truncates the list back to zero length.
 */
void string_array_reset(string_array_t *s);

/* 
 * string_array_free frees the storage associated with the list.
 */
void string_array_free(string_array_t *s);

/* 
 * string_array_add adds a copy of the string |str| to the list.
 */
void string_array_add(string_array_t *s, char *str);

/*
 * string_array_copy copies the strings at src into the list.
 */
void string_array_copy(string_array_t *dst, char **src, int n);

#endif  /* STRING_ARRAY_H_ */
