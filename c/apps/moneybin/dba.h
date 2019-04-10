#ifndef MB_DBA_H_
#define MB_DBA_H_

struct dba_conn_s;

typedef struct dba_driver_ops_s {
  int (*alloc)(struct dba_conn_s *conn);
  int (*free)(struct dba_conn_s *conn);

  int (*connect)(struct dba_conn_s *conn, char const *host, int unsigned port,
                 char const *username, char const *password, char const *dbname);
  int (*disconnect)(struct dba_conn_s *conn);
} dba_driver_ops_t;

typedef struct dba_conn_s {
  dba_driver_ops_t *ops;
  void *data;
} dba_conn_t;

typedef struct dba_s dba_t;

int dba_init(dba_t **handle, char const *backend);
int dba_connect(dba_t *handle, char const *host, int port, char const *username, char const *password, char const *dbname);
int dba_disconnect(dba_t *handle);
int dba_query(dba_t *handle, char const *query, long unsigned length);

#endif  /* MB_DBA_H_ */
