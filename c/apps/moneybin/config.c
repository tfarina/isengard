#include "config.h"

#include "fstrutils.h"
#include "third_party/iniparser/iniparser.h"

int config_init(config_t *config, char const *cfgfile) {
  dictionary *ini;

  ini = iniparser_load(cfgfile);
  if (ini == NULL) {
    fprintf(stderr, "Cannot read configuration file: %s\n", cfgfile);
    return -1;
  }

  config->database = f_strdup(iniparser_getstring(ini, "sql:database", "mysql"));
  config->host = f_strdup(iniparser_getstring(ini, "sql:host", NULL));
  config->user = f_strdup(iniparser_getstring(ini, "sql:user", NULL));
  config->password = f_strdup(iniparser_getstring(ini, "sql:password", NULL));
  config->dbname = f_strdup(iniparser_getstring(ini, "sql:dbname", NULL));
  config->port = iniparser_getint(ini, "sql:port", 3306);

  iniparser_freedict(ini);

  return 0;
}

