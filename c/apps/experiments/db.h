#ifndef DB_H_
#define DB_H_

typedef struct {
  char const *host;
  char const *user;
  char const *password;
  char const *dbname;
  int unsigned port;
} db_config_t;

#endif  /* DB_H_ */
