#include "buffer.h"

#include <stdlib.h>
#include <string.h>

buffer_t *buffer_alloc(size_t capacity)
{
        buffer_t *buf;

	buf = malloc(sizeof(buffer_t));
	if (buf == NULL) {
	        return NULL;
	}

	buf->data = malloc(sizeof(char) * capacity);
        if (buf->data == NULL) {
	        free(buf);
                return NULL;
	}

	buf->capacity = capacity;
	buf->size = 0;

        return buf;
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

        if (self->size + size > self->capacity) {
                size_t const new_capacity = (self->size * 2) + size;
                self->data = realloc(self->data, new_capacity);
                if (self->data == NULL) {
	                return;
	        }

                self->capacity = new_capacity;
	}

	memcpy(self->data + self->size, data, size);
        self->size += size;
        self->data[self->size] = '\0'; /* always 0 terminate data. */
}
