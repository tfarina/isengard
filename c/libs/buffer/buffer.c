#include "buffer.h"

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

	b->data = malloc(sizeof(char) * capacity);
        if (b->data == NULL) {
	        free(b);
                return NULL;
	}

	b->capacity = capacity;
	b->size = 0;

        return b;
}

void buffer_clear(buffer_t *self)
{
        if (self == NULL) {
                return;
        }

	self->size = 0;
	*self->data = 0;
}

void buffer_write(buffer_t *self, void const *data, size_t size)
{
        if (self == NULL) {
                return;
        }

        if (self->capacity < self->size + size) {
          _buffer_realloc(self, self->size * 2 + size);
	}

	memcpy(self->data + self->size, data, size);
        self->size += size;
        self->data[self->size] = '\0'; /* always 0 terminate data. */
}

void buffer_free(buffer_t *self)
{
        if (self == NULL) {
                return;
        }

	if (self->data != NULL) {
                free(self->data);
                self->data = NULL;
	}

        self->size = 0;
        self->capacity = 0;

        free(self);
}
