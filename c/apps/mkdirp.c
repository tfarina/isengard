#include "mkdirp.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "strutils.h"

/**
 * Creates a directory if it doesn't already exist and its intermediate
 * parent directories if needed.
 *
 * Credit:
 * http://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux
 */
int f_mkdirp(char const *pathname, mode_t mode)
{
  char* path;
  char* p;
  int rv = 0;

#if 0
  printf("pathname = %s\n", pathname);
#endif

  if ((pathname == NULL) || (*pathname == '\0')) {
    return -1;
  }

  path = f_strdup(pathname);
  if (path == NULL) {
    return -1;
  }

  p = path;

  while (*p != '\0') {
    /* Skip first character. */
    p++;

    /* Find first slash. */
    while (*p != '\0' && *p != '/') p++;

    /* Remember value from 'p', which should be a slash ('/'). */
    char v = *p;

#if 0
    printf("*p [%c] \n", v);
#endif

    /* Change the slash ('/') by a nul-terminating character ('/0') to indicate the end of the string. */
    *p = '\0';

#if 0
    printf("path [%s] \n", path);
#endif

    /* Create folder from path to '\0' inserted at 'p'. */
    rv = mkdir(path, mode);

    /* Did mkdir return an error other than the path already exists? */
    if (rv != 0 && errno != EEXIST) {
      *p = v;
      rv = -1;
      break;
    }

    /* Restore path to it's former glory, which means putting back the slash ('/') in its place. */
    *p = v;
  }

  free(path);

  return rv;
}
