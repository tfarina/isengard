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
  if (pw == 0) {
    return 0;
  }

  return pw->pw_name;
}
