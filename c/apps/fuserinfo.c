#include "fuserinfo.h"

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

char *f_get_user_name(void)
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
