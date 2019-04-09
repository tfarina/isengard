#include "config.h"

#include "ffileutils.h"
#include "fstrutils.h"
#include "futils.h"
#include "third_party/iniparser/iniparser.h"

#define USERCONFFILE ".moneybinrc"

int config_init(config_t *config) {
  const char *homedir;
  char *userconffile;
  dictionary *ini;

  homedir = f_get_home_dir();
  userconffile = f_build_filename(homedir, USERCONFFILE);

  ini = iniparser_load(userconffile);
  if (ini == NULL) {
    fprintf(stderr, "Cannot read configuration file: %s\n", userconffile);
    return -1;
  }
  
  config->host = f_strdup(iniparser_getstring(ini, "mysql:host", NULL));
  config->user = f_strdup(iniparser_getstring(ini, "mysql:user", NULL));
  config->password = f_strdup(iniparser_getstring(ini, "mysql:password", NULL));
  config->dbname = f_strdup(iniparser_getstring(ini, "mysql:dbname", NULL));
  config->port = iniparser_getint(ini, "mysql:port", 3306);

  iniparser_freedict(ini);

  return 0;
}

