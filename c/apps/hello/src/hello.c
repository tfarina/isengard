#include <config.h>
#include <libintl.h>
#include <stdio.h>

int
main(
  void
  )
{
  setlocale(LC_ALL, "");
  bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
  textdomain(GETTEXT_PACKAGE);

  fputs("Hello, world!\n", stdout);

  return 0;
}
