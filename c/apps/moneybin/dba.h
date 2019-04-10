#ifndef MB_DBA_H_
#define MB_DBA_H_

struct dba_conn_s;

typedef struct dba_driver_ops_s {
  int (*alloc)(struct dba_conn_s *conn);
  int (*free)(struct dba_conn_s *conn);

  int (*connect)(struct dba_conn_s *conn);
  int (*disconnect)(struct dba_conn_s *conn);
} dba_driver_ops_t;

typedef struct dba_conn_s {
  char const *host;
  char const *user;
  char const *password;
  char const *dbname;
  int unsigned port;
  dba_driver_ops_t *ops;
  void *data;
} dba_conn_t;

int dba_conn_alloc(dba_conn_t *conn);
int dba_conn_free(dba_conn_t *conn);

int dba_conn_connect(dba_conn_t *conn);
int dba_conn_disconnect(dba_conn_t *conn);

typedef struct dba_s dba_t;

int dba_init(dba_t **handle, char const *backend);
int dba_connect(dba_t *handle, char const *host, int port, char const *username, char const *password, char const *dbname);
int dba_disconnect(dba_t *handle);
int dba_query(dba_t *handle, char const *query, long unsigned length);

#endif  /* MB_DBA_H_ */
