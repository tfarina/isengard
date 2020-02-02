#ifndef _OPTION_H_INCLUDED_
#define _OPTION_H_INCLUDED_ 1

typedef struct option_s {
  int unsigned detach:1;       /* process: foreground mode */
  char *conffile;      /* process: configuration filename */
  char *logfile;       /* debug:   log filename */
  char *pidfile;       /* process: pid file */
  char *user;          /* process: run as another user */
  char *address;       /* network: local address to bind */
  int  port;           /* network: tcp listening port */
  int  backlog;        /* network: tcp backlog */
} option_t;

/**
 * Initialize |opt| to default values.
 */
void option_init(option_t *opt);

#endif  /* !defined(_OPTION_H_INCLUDED_) */
