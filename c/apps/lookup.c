/**
 * Based on:
 * www.nlnetlabs.nl/svn/nsd/trunk/util.c
 *
 */

#include <stdio.h>

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

struct priority_info {
        int id;
        const char *name;
};

static struct priority_info log_priority_info_table[] = {
        { LOG_INFO, "info" },
        { LOG_WARNING, "warning" },
        { LOG_ERROR, "error" },
        { LOG_FATAL, "fatal" },
        { 0, NULL }
};

static struct priority_info*
get_priority_info_by_id(struct priority_info table[], int id)
{
        struct priority_info *item;

        for (item = table; item; item++) {
                if (item->id == id) {
                        return item;
                }
        }
        return NULL;
}

int main(void) {
        struct priority_info *pinfo;

        pinfo = get_priority_info_by_id(log_priority_info_table, LOG_FATAL);

        if (pinfo) {
                printf("%s\n", pinfo->name);
        }

        printf("%s\n", get_log_severity_name(LOG_ERROR));

        return 0;
}
