#ifndef MB_DBA_H_
#define MB_DBA_H_

typedef enum dba_err_e {
  DBA_ERR_SUCCESS = 0,
  DBA_ERR_PARAM,
  DBA_ERR_NOMEM,
  DBA_ERR_NOOP,
  DBA_ERR_NOTEXIST,
  DBA_ERR_HANDLE,
  DBA_ERR_BACKEND,
  DBA_MAX_ERRNO
} dba_err_t;

typedef struct dba_s dba_t;
typedef struct dba_ops_s dba_ops_t;
typedef struct dba_result_s dba_result_t;

struct dba_s {
  dba_ops_t *ops;
  void *data;
};

struct dba_ops_s {
  int (*init)(dba_t *handle);
  int (*deinit)(dba_t *handle);
  int (*connect)(dba_t *handle, char const *host, int unsigned port,
                 char const *username, char const *password, char const *dbname);
  int (*disconnect)(dba_t *handle);
  int (*query)(dba_t *handle, char const *query, long unsigned length);
  int (*result)(dba_t *handle, dba_result_t **result);
};

struct dba_result_s {
  dba_t *handle;
  void *data;
};

int dba_init(dba_t **handle, char const *backend);
int dba_deinit(dba_t *handle);
char *dba_strerror(int const dba_errno);
int dba_connect(dba_t *handle, char const *host, int unsigned port,
                char const *username, char const *password, char const *dbname);
int dba_disconnect(dba_t *handle);
int dba_query(dba_t *handle, char const *query, long unsigned length);
int dba_result(dba_t *handle, dba_result_t **result);

extern dba_ops_t mysql_dba_ops;

#endif  /* MB_DBA_H_ */
