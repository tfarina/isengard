#ifndef STRING_LIST_H_
#define STRING_LIST_H_

/*
 * string_list_t is like Go's []string.
 */
typedef struct {
	char **strs;
	int len;
	int cap;
} string_list_t;

/*
 * string_list_init prepares an uninitialized list for use.
 */
void string_list_init(string_list_t *s);

/*
 * string_list_reset truncates the list back to zero length.
 */
void string_list_reset(string_list_t *s);

/* 
 * string_list_free frees the storage associated with the list.
 */
void string_list_free(string_list_t *s);

/* 
 * string_list_add adds a copy of the string |str| to the list.
 */
void string_list_add(string_list_t *s, char *str);

/*
 * string_list_copy copies the strings at src into the list.
 */
void string_list_copy(string_list_t *dst, char **src, int n);

#endif  /* STRING_LIST_H_ */
