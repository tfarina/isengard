#ifndef _OPTIONS_H_INCLUDED_
#define _OPTIONS_H_INCLUDED_ 1

/**
 * Where to keep state files.
 */
#define DEF_STATEDIR "/var/run/echod"

typedef struct options_s {
  int unsigned detach:1; /* foreground mode */
  char *conffile;        /* configuration filename */
  char *logfile;         /* log filename */
  char *pidfile;         /* pid filename */
  char *user;            /* user to switch to */
  char *address;         /* local address to bind */
  int   port;            /* tcp listening port */
  int   backlog;         /* tcp backlog */
  int unsigned dump_options:1;
} options_t;

extern options_t opt;

/**
 * Initialize |opt| to default values.
 */
void init_options(void);

/**
 * Dump command-line options to standard output for testing.
 */
void dump_options(void);

void preparse_args(int argc, char **argv);

/**
 * Parse command-line arguments into |opt|.
 *
 * @param   argc   Size of @a argv.
 * @param   argv   List of arguments.
 */
void parse_args(int argc, char **argv);

#endif  /* !defined(_OPTIONS_H_INCLUDED_) */
