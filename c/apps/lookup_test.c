#include <stdio.h>

#include "lookup.h"

typedef enum {
        LOG_INFO = 0,
        LOG_WARNING = 1,
        LOG_ERROR = 2,
        LOG_FATAL = 3,
        LOG_COUNT = 4
} log_level_t;

static lookup_entry_t log_level_table[] = {
        { LOG_INFO, "info" },
        { LOG_WARNING, "warning" },
        { LOG_ERROR, "error" },
        { LOG_FATAL, "fatal" },
        { 0, NULL }
};

int
main(void)
{
        lookup_entry_t *item;

	item = lookup_entry_by_id(log_level_table, LOG_FATAL);
	if (item) {
	        printf("%s\n", item->name);
	}

        return 0;
}
