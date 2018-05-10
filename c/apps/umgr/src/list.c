// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

#include <stdio.h>
#include <stdlib.h>

#include "user.h"

int main(int argc, char **argv) {
  int rv;

  rv = user_list();

  return rv;
}
