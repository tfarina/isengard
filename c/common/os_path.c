#include "os_path.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DIR_SEP '/'

char const *os_path_basename(char const *path) {
  char const *last_slash;

  /* Search for the last slash in path. */
  last_slash = strrchr(path, DIR_SEP);
  /* If there is no DIR_SEP in path, the basename is path. */
  if (last_slash == 0) {
    return path;
  }

  return last_slash + 1; /* Move the pointer past the '/'. */
}

char *
os_path_join(char *dir, char *file)
{
  char *path;
  size_t len;

  /*
   * Add 2 bytes to the resulting length to have enough memory available
   * for the directory separator ('/') and the null byte ('\0').
   */
  len = strlen(dir) + strlen(file) + 2;

  path = malloc(sizeof(char) * len);
  if (path == NULL) {
    return NULL;
  }

  strcpy(path, dir);
  strcat(path, "/");
  strcat(path, file);

  return path;
}

int os_path_exists(char const *path)
{
  struct stat sb;
  int rv;

  rv = stat(path, &sb);

  return rv == 0;
}

int os_path_isdir(char const *path)
{
  struct stat sb;
  int rc;

  rc = stat(path, &sb);

  return rc == 0 && S_ISDIR(sb.st_mode);
}
