#ifndef MB_DBA_H_
#define MB_DBA_H_

struct dba_conn_s;

typedef struct dba_driver_ops_s {
  char *name;

  int (*alloc)(struct dba_conn_s *conn);
  int (*free)(struct dba_conn_s *conn);

  int (*connect)(struct dba_conn_s *conn);
  int (*disconnect)(struct dba_conn_s *conn);
} dba_driver_ops_t;

typedef struct dba_driver_s {
  dba_driver_ops_t *ops;
} dba_driver_t;

typedef struct dba_conn_s {
  char const *host;
  char const *user;
  char const *password;
  char const *dbname;
  int unsigned port;
  dba_driver_t *driver;
  void *data;
} dba_conn_t;

int dba_conn_alloc(dba_conn_t *conn);
int dba_conn_free(dba_conn_t *conn);

int dba_conn_connect(dba_conn_t *conn);
int dba_conn_disconnect(dba_conn_t *conn);

#endif  /* MB_DBA_H_ */
