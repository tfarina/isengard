#ifndef MB_DBA_H_
#define MB_DBA_H_

/**
 * Error Handling: this api follows the error reporting conventions of
 * the errno.h. Functions return a non-error code to indicate an error
 * and 0 to indicate success. You should check the return status of the
 * functions that you call.
 */
enum {
  DBA_SUCCESS = 0,  /**< No error */
  DBA_EINVAL,       /**< Invalid argument */
  DBA_ENOMEM,       /**< Out of memory */
  DBA_ENOOP,
  DBA_ENOTEXIST,
  DBA_EHANDLE,
  DBA_EBACKEND,
  DBA_MAX_ERRNO
};

/** DBA result return values */
typedef enum dba_result_code_e {
  DBA_RES_DONE,
  DBA_RES_NOROWS,
  DBA_RES_ROWS
} dba_result_code_t;

/** DBA row return values */
typedef enum dba_row_code_e {
  DBA_ROW_DONE,
  DBA_ROW_NEXT
} dba_row_code_t;

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
  int (*result_init)(dba_t *handle, dba_result_t **result);
  int (*result_deinit)(dba_result_t *result);
  int (*result_fetch_row)(dba_result_t *result);
  char const *(*result_get_field_value)(dba_result_t *result, long unsigned index);
};

struct dba_result_s {
  dba_t *handle;
  void *data;
};

int dba_init(dba_t **handle, char const *backend);
int dba_deinit(dba_t *handle);
char const *dba_strerror(int const dba_errno);
int dba_connect(dba_t *handle, char const *host, int unsigned port,
                char const *username, char const *password, char const *dbname);
int dba_disconnect(dba_t *handle);
int dba_query(dba_t *handle, char const *query, long unsigned length);
int dba_result_init(dba_t *handle, dba_result_t **result);
int dba_result_deinit(dba_result_t *result);
int dba_result_fetch_row(dba_result_t *result);
char const *dba_result_get_field_value(dba_result_t *result, long unsigned index);

#endif  /* MB_DBA_H_ */
