#ifndef ED_CONFIG_H_
#define ED_CONFIG_H_

#define ED_USER "_echod"
#define ED_MAXIMIZE_COREFILE 0
#define ED_INTERFACE NULL
#define ED_TCP_PORT 7
#define ED_BACKLOG 1024

typedef struct ed_config_s {
  int  daemonize;      /* process: daemon mode */
  char *username;      /* process: run as another user */
  char *log_filename;  /* debug:   log filename */
  char *pid_filename;  /* process: pid file */
  char *interface;     /* network: listening interface */
  int  port;           /* network: tcp listening port */
  int  backlog;        /* network: tcp backlog */
} ed_config_t;

void ed_config_init(ed_config_t *config);

#endif  /* ED_CONFIG_H_ */
