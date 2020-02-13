#ifndef _OPTIONS_H_INCLUDED_
#define _OPTIONS_H_INCLUDED_ 1

typedef struct options_s {
  int unsigned detach:1;       /* process: foreground mode */
  char *conffile;      /* process: configuration filename */
  char *logfile;       /* debug:   log filename */
  char *pidfile;       /* process: pid file */
  char *user;          /* process: run as another user */
  char *address;       /* network: local address to bind */
  int  port;           /* network: tcp listening port */
  int  backlog;        /* network: tcp backlog */
} options_t;

/**
 * Initialize |opt| to default values.
 */
void init_options(options_t *opt);

#endif  /* !defined(_OPTIONS_H_INCLUDED_) */
