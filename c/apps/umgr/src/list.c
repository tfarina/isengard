// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

#include "user.h"

int main(int argc, char **argv) {
  int rc;

  rc = user_list();

  return rc;
}
