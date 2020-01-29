#ifndef ED_CMDLINE_H_
#define ED_CMDLINE_H_

#include "ed_config.h"

/**
 * Parse command-line options from arguments given into |config|.
 */
void parse_args(int argc, char **argv, ed_config_t *config);

#endif  /* ED_CMDLINE_H_ */
