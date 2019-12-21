#ifndef ED_CMDLINE_H_
#define ED_CMDLINE_H_

#include "ed_config.h"

extern int show_help;
extern int show_version;

int ed_cmdline_parse(int argc, char **argv, char const *program_name,
                     ed_config_t *config);

void ed_cmdline_help(char const *program_name);

void ed_cmdline_display_version(void);

#endif  /* ED_CMDLINE_H_ */
