#ifndef VEC_H_
#define VEC_H_

/*
 * The invariant with the vectors is that v->strs[0:v->len] is allocated
 * strings that are owned by the vector. The data beyond v->len may be
 * garbage.
 */

/*
 * A vec_t is a string vector, like Go's []string.
 */
typedef struct {
	char **strs;
	int len;
	int cap;
} vec_t;

/*
 * vec_init prepares an uninitialized vector for use.
 */
void vec_init(vec_t *v);

/*
 * vec_reset truncates the vector back to zero length.
 */
void vec_reset(vec_t *v);

/* 
 * vec_free frees the storage associated with the vector.
 */
void vec_free(vec_t *v);

/* 
 * vec_add adds a copy of the string p to the vector.
 */
void vec_add(vec_t *v, char *p);

/*
 * vec_copy copies the srclen strings at src into the vector.
 */
void vec_copy(vec_t *dst, char **src, int n);

#endif  /* VEC_H_ */
