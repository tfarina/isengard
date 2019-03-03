#include "db.h"

#include "ffileutils.h"
#include "futils.h"
#include "third_party/iniparser/iniparser.h"

#define USERCONFFILE ".experimentsrc"

void db_config_init(db_config_t *config) {
  const char *homedir;
  char *userconffile;
  dictionary *ini;

  homedir = f_get_home_dir();
  userconffile = f_build_filename(homedir, USERCONFFILE);

  ini = iniparser_load(userconffile);

  config->host = strdup(iniparser_getstring(ini, "mysql:host", NULL));
  config->port = 0;
  config->user = strdup(iniparser_getstring(ini, "mysql:user", NULL));
  config->password = strdup(iniparser_getstring(ini, "mysql:password", NULL));
  config->dbname = strdup(iniparser_getstring(ini, "mysql:dbname", NULL));

  iniparser_freedict(ini);
}

