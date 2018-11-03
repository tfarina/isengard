#include "lookup.h"

#include <stdlib.h>

lookup_entry_t *
lookup_entry_by_id(lookup_entry_t table[], int id)
{
        lookup_entry_t *item;

	for (item = table; item; item++) {
   	        if (item->id == id) {
  	                return item;
	        }
	}

	return NULL;
}
