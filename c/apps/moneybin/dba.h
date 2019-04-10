#ifndef MB_DBA_H_
#define MB_DBA_H_

typedef struct dba_s dba_t;
typedef struct dba_ops_s dba_ops_t;

struct dba_s {
  dba_ops_t *ops;
  void *data;
};

struct dba_ops_s {
  int (*init)(dba_t *handle);
  int (*connect)(dba_t *handle, char const *host, int unsigned port,
                 char const *username, char const *password, char const *dbname);
  int (*disconnect)(dba_t *handle);
  int (*query)(dba_t *handle, char const *query, long unsigned length);
};

int dba_init(dba_t **handle, char const *backend);
int dba_connect(dba_t *handle, char const *host, int unsigned port,
                char const *username, char const *password, char const *dbname);
int dba_disconnect(dba_t *handle);
int dba_query(dba_t *handle, char const *query, long unsigned length);

#endif  /* MB_DBA_H_ */
