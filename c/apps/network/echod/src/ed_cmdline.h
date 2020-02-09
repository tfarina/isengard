#ifndef ED_CMDLINE_H_
#define ED_CMDLINE_H_

#include "option.h"

void preparse_args(int argc, char **argv, option_t *opt);

/**
 * Parse command-line options from arguments given into |opt|.
 */
void parse_args(int argc, char **argv, option_t *opt);

#endif  /* ED_CMDLINE_H_ */
