#include "dba.h"

#include <stdlib.h>
#include <string.h>

extern dba_ops_t mysql_dba_ops;

typedef struct dba_backend_s {
  char const *backend;
  dba_ops_t *ops;
} dba_backend_t;

static dba_backend_t dba_backend_table[] = {
  { "mysql", &mysql_dba_ops },
  { NULL, NULL }
};

static int
dba_backend_lookup(char const *backend, dba_backend_t **out_dbe)
{
  dba_backend_t *current;

  for (current = dba_backend_table; current != NULL; current++) {
    if (strcmp(current->backend, backend) == 0) {
      *out_dbe = current;
      return DBA_SUCCESS;
    }
  }

  return -DBA_ENOTEXIST;
}

int
dba_init(dba_t **handle, char const *backend)
{
  int err;
  dba_backend_t *dba_backend = NULL;

  if (handle == NULL || backend == NULL) {
    return -DBA_EINVAL;
  }

  *handle = malloc(sizeof(dba_t));
  if (*handle == NULL) {
    return -DBA_ENOMEM;
  }

  (*handle)->ops = NULL;
  (*handle)->data = NULL;

  err = dba_backend_lookup(backend, &dba_backend);
  if (err == DBA_SUCCESS) {
    err = -DBA_ENOOP;

    (*handle)->ops = dba_backend->ops;

    if ((*handle)->ops && (*handle)->ops->init) {
      err = (*handle)->ops->init(*handle);
      if (err == DBA_SUCCESS) {
	return DBA_SUCCESS;
      }
    }
  }

  free(*handle);
  *handle = NULL;

  return err;
}

int
dba_deinit(dba_t *handle)
{
  int err;

  if (handle != NULL) {
    if (handle->ops != NULL && handle->ops->deinit != NULL) {
      err = handle->ops->deinit(handle);
      if (err < 0) {
	return err;
      }
    }

    handle->ops = NULL;
    free(handle);

    return DBA_SUCCESS;
  }

  return -DBA_EHANDLE;
}

char const *
dba_strerror(int const dba_errno)
{
  switch(dba_errno) {
  case DBA_SUCCESS:
    return "success";
  case DBA_EINVAL:
    return "invalid parameter";
  case DBA_ENOMEM:
    return "out of memory";
  case DBA_ENOOP:
    return "operation is not available";
  case DBA_ENOTEXIST:
    return "loading backend library failed";
  case DBA_EHANDLE:
    return "invalid handle";
  case DBA_EBACKEND:
    return "backend error occured";
  }

  return "unknown error code";
}

int
dba_connect(dba_t *handle, char const *host, int unsigned port,
            char const *username, char const *password, char const *dbname)
{
  if (handle == NULL) {
    return -DBA_EHANDLE;
  }

  if (handle->ops != NULL && handle->ops->connect != NULL) {
    return handle->ops->connect(handle, host, port, username, password, dbname);
  }

  return DBA_SUCCESS;
}

int
dba_disconnect(dba_t *handle)
{
  if (handle == NULL) {
    return -DBA_EHANDLE;
  }

  if (handle->ops != NULL && handle->ops->disconnect != NULL) {
    return handle->ops->disconnect(handle);
  }

  return DBA_SUCCESS;
}

int
dba_query(dba_t *handle, char const *query, long unsigned length)
{
  if (handle == NULL) {
    return -DBA_EHANDLE;
  }

  if (query == NULL) {
    return -DBA_EINVAL;
  }

  if (length == 0) {
    length = (long unsigned)strlen(query);
  }

  if (handle->ops != NULL && handle->ops->query != NULL) {
    return handle->ops->query(handle, query, length);
  }

  return DBA_SUCCESS;
}

int
dba_result_init(dba_t *handle, dba_result_t **result)
{
  if (handle == NULL) {
    return -DBA_EHANDLE;
  }

  if (handle->ops != NULL && handle->ops->result_init != NULL) {
    int err;
    *result = NULL;

    err = handle->ops->result_init(handle, result);
    if (err > DBA_RES_DONE) {
      if (*result) {
	(*result)->handle = handle;
      }
    } else {
	*result = NULL;
    }

    return err;
  }
  
  return DBA_SUCCESS;
}

int
dba_result_deinit(dba_result_t *result)
{
  if (result == NULL || result->handle == NULL) {
    return -DBA_EHANDLE;
  }

  if (result->handle->ops != NULL && result->handle->ops->result_deinit != NULL) {
    return result->handle->ops->result_deinit(result);
  }

  return DBA_SUCCESS;
}

int
dba_result_fetch_row(dba_result_t *result)
{
  if (result == NULL || result->handle == NULL) {
    return -DBA_EHANDLE;
  }

  if (result->handle->ops != NULL && result->handle->ops->result_fetch_row != NULL) {
    return result->handle->ops->result_fetch_row(result);
  }

  return DBA_SUCCESS;
}

char const *
dba_result_get_field_value(dba_result_t *result, long unsigned index)
{
  if (result == NULL || result->handle == NULL) {
    return NULL;
  }

  if (result->handle->ops != NULL && result->handle->ops->result_get_field_value != NULL) {
    return result->handle->ops->result_get_field_value(result, index);
  }

  return NULL;
}
