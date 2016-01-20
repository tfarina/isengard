/**
 * Based on:
 * www.nlnetlabs.nl/svn/nsd/trunk/util.c
 *
 */

#include <stdio.h>

enum {
        LOG_WARNING = 0,
        LOG_INFO = 1,
};

struct priority_info {
  int id;
  const char *name;
};

static struct priority_info log_priority_info_table[] = {
        { LOG_WARNING, "warning" },
        { LOG_INFO, "info" },
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

        pinfo = get_priority_info_by_id(log_priority_info_table, LOG_INFO);

        if (pinfo) {
                printf("%s\n", pinfo->name);
        }

        return 0;
}
