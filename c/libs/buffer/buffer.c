#include "buffer.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * _buffer_realloc ensures that the buffer has at least |length| more bytes between its
 * length and capacity.
 */
static void _buffer_realloc(buffer_t *self, size_t capacity)
{
        void *temp;

        temp = realloc(self->data, capacity);
        if (temp == NULL) {
	        return;
	}

        self->data = temp;
        self->capacity = capacity;
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

void buffer_reset(buffer_t *self)
{
	self->length = 0;
	*self->data = 0;
}

void buffer_write(buffer_t *self, void const *data, size_t length)
{
        if (self->capacity < self->length + length) {
          _buffer_realloc(self, self->length * 2 + length);
	}
	memcpy(self->data + self->length, data, length);
        self->length += length;
        self->data[self->length] = '\0'; /* always 0 terminate data. */
}

void buffer_free(buffer_t *self)
{
        free(self->data);
        self->data = NULL;
        self->length = 0;
        free(self);
}
