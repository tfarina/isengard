#ifndef ED_CONFIG_H_
#define ED_CONFIG_H_

 /* Group for running the Echo daemon. */
#define ED_GROUP "_echod"

 /* User for running the Echo daemon. */
#define ED_USER "_echod"

#define ED_DEFAULT_LISTEN_ADDR "0.0.0.0"
#define ED_DEFAULT_LISTEN_PORT 7
#define ED_DEFAULT_LISTEN_BACKLOG 1024

typedef struct ed_config_s {
  int pid;             /* process: server id */
  int  daemonize:1;    /* process: daemon mode */
  char *conffile;      /* process: configuration filename */
  char *logfile;       /* debug:   log filename */
  char *pidfile;       /* process: pid file */
  char *username;      /* process: run as another user */
  char *address;       /* network: local address to bind */
  int  port;           /* network: tcp listening port */
  int  backlog;        /* network: tcp backlog */
} ed_config_t;

/**
 * Initialize |config| to default values.
 */
void ed_config_init(ed_config_t *config);

/**
 * Read configuration data from config->conffile into |config|.
 */
int ed_config_load_file(ed_config_t *config);

#endif  /* ED_CONFIG_H_ */
