#include "buffer.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * _buffer_realloc ensures that the buffer has at least |length| more bytes between its
 * length and capacity.
 */
static void _buffer_realloc(buffer_t *b, size_t capacity)
{
        void *temp;

        temp = realloc(b->data, capacity);
        if (temp == NULL) {
	        return;
	}

        b->data = temp;
        b->capacity = capacity;
}

buffer_t *buffer_alloc(size_t capacity)
{
        buffer_t *b;

	b = malloc(sizeof(buffer_t));

	if (b == NULL) {
	        return NULL;
	}

	b->data = malloc(capacity);
        if (b->data == NULL) {
	        free(b);
                return NULL;
	}

	b->capacity = capacity;
	b->length = 0;

        return b;
}

void buffer_free(buffer_t *b)
{
        free(b->data);
        free(b);
}

void buffer_reset(buffer_t *b)
{
	b->length = 0;
	*b->data = 0;
}

void buffer_write(buffer_t *b, void const *data, size_t length)
{
        if (b->capacity < b->length + length) {
          _buffer_realloc(b, b->length * 2 + length);
	}
	memcpy(b->data + b->length, data, length);
        b->length += length;
        b->data[b->length] = '\0'; /* always 0 terminate data. */
}
