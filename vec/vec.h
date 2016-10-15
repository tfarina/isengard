#ifndef VEC_H_
#define VEC_H_

// The invariant with the vectors is that v->strs[0:v->len] is allocated
// strings that are owned by the vector. The data beyond v->len may be
// garbage.

// A Vec is a string vector, like Go's []string.
typedef struct Vec Vec;
struct Vec
{
	char **strs;
	int len;
	int cap;
};

// vec_init prepares an uninitialized vector for use.
void vec_init(Vec *v);

// vec_reset truncates the vector back to zero length.
void vec_reset(Vec *v);

// vec_free frees the storage associated with the vector.
void vec_free(Vec *v);

// vec_add adds a copy of the string p to the vector.
void vec_add(Vec *v, char *p);

// vec_copy copies the srclen strings at src into the vector.
void vec_copy(Vec *dst, char **src, int n);

#endif  // VEC_H_
