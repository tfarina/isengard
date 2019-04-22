#ifndef CONFIG_H_
#define CONFIG_H_

typedef struct {
  char const *database;
  char const *host;
  char const *user;
  char const *password;
  char const *dbname;
  int unsigned port;
} config_t;

int config_init(config_t *config, char const *cfgfile);

#endif  /* CONFIG_H_ */
