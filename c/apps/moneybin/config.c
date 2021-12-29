#include "config.h"

#include "fstrdup.h"
#include "third_party/iniparser/iniparser.h"
#include "third_party/libconfigini/configini.h"
#include "third_party/ini/ini.h"

int config_alloc(config_t **config) {
  config_t *cfg;

  cfg = calloc(1, sizeof(*cfg));
  if (cfg == NULL) {
    return -1 /*ENOMEM*/;
  }

  if (config) {
    *config = cfg;
  }

  return 0;
}

void config_free(config_t *config) {
  free(config);
}

int config_init(config_t *config) {
  config->database = NULL;
  config->host = NULL;
  config->user = NULL;
  config->password = NULL;
  config->dbname = NULL;
  config->port = 0;

  return 0;
}

int config_load(config_t *config, char const *cfgfile) {
  dictionary *ini;

  ini = iniparser_load(cfgfile);
  if (ini == NULL) {
    fprintf(stderr, "Cannot read configuration file: %s\n", cfgfile);
    return -1;
  }

  /* NOTE: after iniparser_freedict is called the memory used
   * for the values will be freed and the pointers will point to garbage.
   * Hence, we need to duplicate the strings into other regions of memory,
   * so they are globally available during the execution of the program.
   */
  config->database = f_strdup(iniparser_getstring(ini, "sql:database", "mysql"));
  config->host = f_strdup(iniparser_getstring(ini, "sql:host", NULL));
  config->user = f_strdup(iniparser_getstring(ini, "sql:user", NULL));
  config->password = f_strdup(iniparser_getstring(ini, "sql:password", NULL));
  config->dbname = f_strdup(iniparser_getstring(ini, "sql:dbname", NULL));
  config->port = iniparser_getint(ini, "sql:port", 3306);

  iniparser_freedict(ini);

  return 0;
}

int config_load_r(config_t *config, char const *cfgfile) {
  Config *cfg;
  ConfigRet cfg_ret;
  char str[1024];

  cfg = ConfigNew();
  if (cfg == NULL) {
    fprintf(stderr, "Failed to allocate memory for Config\n");
    return -1;
  }

  cfg_ret = ConfigReadFile(cfgfile, &cfg);
  if (cfg_ret != CONFIG_OK) {
    fprintf(stderr, "Cannot read configuration file '%s': %s\n", cfgfile, ConfigRetToString(cfg_ret));
    return -1;
  }

  ConfigReadString(cfg, "sql", "database", str, sizeof(str), "mysql");
  config->database = f_strdup(str);

  ConfigReadString(cfg, "sql", "host", str, sizeof(str), NULL);
  config->host = f_strdup(str);

  ConfigReadString(cfg, "sql", "user", str, sizeof(str), NULL);
  config->user = f_strdup(str);

  ConfigReadString(cfg, "sql", "password", str, sizeof(str), NULL);
  config->password = f_strdup(str);

  ConfigReadString(cfg, "sql", "dbname", str, sizeof(str), NULL);
  config->dbname = f_strdup(str);

  ConfigReadUnsignedInt(cfg, "sql", "port", &config->port, 3306);

  ConfigFree(cfg);

  return 0;
}

int config_load_r2(config_t *config, char const *cfgfile) {
  ini_t *handle;
  char *value = NULL;

  handle = ini_load(cfgfile);
  if (handle == NULL) {
    fprintf(stderr, "Cannot read configuration file '%s'\n", cfgfile);
    return -1;
  }

  ini_read_str(handle, "sql", "database", &value, "mysql");
  config->database = value;

  ini_read_str(handle, "sql", "host", &value, "");
  config->host = value;

  ini_read_str(handle, "sql", "user", &value, "");
  config->user = value;

  ini_read_str(handle, "sql", "password", &value, "");
  config->password = value;

  ini_read_str(handle, "sql", "dbname", &value, "");
  config->dbname = value;

  ini_read_unsigned(handle, "sql", "port", &config->port, 3306);

  ini_free(handle);

  return 0;
}
