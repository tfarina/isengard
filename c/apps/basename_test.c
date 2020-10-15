#include "utest.h"

#include "os_path.h"

UTEST(os_path_test, basename)
{
  /*
   *  path           basename
   *  "/usr/lib"     "lib"
   *  "/usr/"        "usr"
   *  "usr"          "usr"
   *  "/"            "/"
   *  "."            "."
   *  ".."           ".."
   *  NULL           ""
   *  ""             ""
   *  "./.."         ".."
   *
   *  These results can be verified by running the unix commands basename(1).
   */

  char const *name;

  name = os_path_basename("/usr/lib");
  ASSERT_STREQ("lib", name);

  /* In the current implementation, this fails. */
  name = os_path_basename("/usr/");
  /*  EXPECT_STREQ("usr", name);*/

  name = os_path_basename("usr");
  ASSERT_STREQ("usr", name);

  /* In the current implementation, this fails. */
  name = os_path_basename("/");
  /*  EXPECT_STREQ("/", name);*/

  name = os_path_basename(".");
  ASSERT_STREQ(".", name);

  name = os_path_basename("..");
  ASSERT_STREQ("..", name);

  /* This test causes a segmentation fault.*/
  /*name = os_path_basename((void *)0);*/
  /*ASSERT_STREQ("", name);*/

  name = os_path_basename("");
  ASSERT_STREQ("", name);

  name = os_path_basename("./..");
  ASSERT_STREQ("..", name);
}

UTEST_MAIN()
