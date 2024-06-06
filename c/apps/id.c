#include "id.h"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * Returns the user's login name.
 */
char *
id_username(void)
{
  uid_t uid;
  struct passwd *pw;

  uid = getuid();
  pw = getpwuid(uid);
  if (NULL == pw) {
    return NULL;
  }

  return pw->pw_name;
}
