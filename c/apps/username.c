#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Gets the user name of the current user.
 */
static const char *get_user_name(void)
{
  struct passwd *pw;

  pw = getpwuid(geteuid());

  return pw->pw_name;
}

int main(int argc, char **argv) {
  const char *username;

  username = get_user_name();

  printf("%s\n", username);

  return 0;
}
