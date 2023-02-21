#include "utest.h"

#include "mkdirp.h"
#include "os_path.h"

UTEST(mkdirp_test, create_parents)
{
  if (os_path_exists("./tmp/foo/bar/baz")) { rmdir("./tmp/foo/bar/baz"); }
  if (os_path_exists("./tmp/foo/bar/"))    { rmdir("./tmp/foo/bar/");    }
  if (os_path_exists("./tmp/foo/"))        { rmdir("./tmp/foo/");        }
  if (os_path_exists("./tmp/"))            { rmdir("./tmp/");            }

  ASSERT_EQ(0, f_mkdirp("./tmp/foo/bar/baz", 0700));
  ASSERT_TRUE(os_path_exists("./tmp"));
  ASSERT_TRUE(os_path_exists("./tmp/foo"));
  ASSERT_TRUE(os_path_exists("./tmp/foo/bar"));
  ASSERT_TRUE(os_path_exists("./tmp/foo/bar/baz"));
}

UTEST_MAIN()
