#ifndef ED_CONFIG_H_
#define ED_CONFIG_H_

typedef struct ed_config_s {
  int unsigned detach:1;       /* process: foreground mode */
  char *conffile;      /* process: configuration filename */
  char *logfile;       /* debug:   log filename */
  char *pidfile;       /* process: pid file */
  char *user;          /* process: run as another user */
  char *address;       /* network: local address to bind */
  int  port;           /* network: tcp listening port */
  int  backlog;        /* network: tcp backlog */
} ed_config_t;

/**
 * Initialize |config| to default values.
 */
void ed_config_init(ed_config_t *config);

#endif  /* ED_CONFIG_H_ */
