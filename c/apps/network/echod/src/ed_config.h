#ifndef ED_CONFIG_H_
#define ED_CONFIG_H_

#define ED_USER "_echod"
#define ED_MAXIMIZE_COREFILE 0
#define ED_DEFAULT_LISTEN_ADDR "0.0.0.0"
#define ED_DEFAULT_LISTEN_PORT 7
#define ED_DEFAULT_LISTEN_BACKLOG 1024

typedef struct ed_config_s {
  int  daemonize;      /* process: daemon mode */
  char *conffile;      /* process: configuration filename */
  char *logfile;       /* debug:   log filename */
  char *pidfile;       /* process: pid file */
  char *username;      /* process: run as another user */
  char *interface;     /* network: listening interface */
  int  port;           /* network: tcp listening port */
  int  backlog;        /* network: tcp backlog */
} ed_config_t;

void ed_config_init(ed_config_t *config);

#endif  /* ED_CONFIG_H_ */
