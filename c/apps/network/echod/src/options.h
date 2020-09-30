#ifndef _OPTIONS_H_INCLUDED_
#define _OPTIONS_H_INCLUDED_ 1

typedef struct options_s {
  int unsigned detach:1; /* foreground mode */
  char *conffile;        /* configuration filename */
  char *logfile;         /* log filename */
  char *pidfile;         /* pid filename */
  char *user;            /* user to switch to */
  char *address;         /* local address to bind */
  int   port;            /* tcp listening port */
  int   backlog;         /* tcp backlog */
} options_t;

extern options_t opt;

/**
 * Initialize |opt| to default values.
 */
void init_options(void);

void preparse_args(int argc, char **argv);

/**
 * Parse command-line options from arguments into |opt|.
 */
void parse_args(int argc, char **argv);

#endif  /* !defined(_OPTIONS_H_INCLUDED_) */
