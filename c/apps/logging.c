/**
 * Based on:
 * www.nlnetlabs.nl/svn/nsd/trunk/util.c
 *
 */

#include <stdio.h>

#include "lookup.h"

enum log_severity {
        LOG_INFO = 0,
        LOG_WARNING = 1,
        LOG_ERROR = 2,
        LOG_FATAL = 3,
        LOG_NUM_SEVERITIES = 4,
};

const char *const log_severity_names[LOG_NUM_SEVERITIES] = {
        "info",
        "warning",
        "error",
        "fatal",
};

const char *
get_log_severity_name(int severity)
{
        if (severity >= 0 && severity < LOG_NUM_SEVERITIES)
                return log_severity_names[severity];
        return "unknown";
}

static lookup_entry_t log_priority_table[] = {
        { LOG_INFO, "info" },
        { LOG_WARNING, "warning" },
        { LOG_ERROR, "error" },
        { LOG_FATAL, "fatal" },
        { 0, NULL }
};

int
main(void)
{
        lookup_entry_t *priority_entry;

        priority_entry = lookup_entry_by_id(log_priority_table, LOG_FATAL);

        if (priority_entry) {
                printf("%s\n", priority_entry->name);
        }

        printf("%s\n", get_log_severity_name(LOG_ERROR));

        return 0;
}
