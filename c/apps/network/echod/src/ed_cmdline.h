#ifndef ED_CMDLINE_H_
#define ED_CMDLINE_H_

#include "options.h"

void preparse_args(int argc, char **argv, options_t *opt);

/**
 * Parse command-line options from arguments given into |opt|.
 */
void parse_args(int argc, char **argv, options_t *opt);

#endif  /* ED_CMDLINE_H_ */
