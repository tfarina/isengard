#include <config.h>
#include <libintl.h>
#include <stdio.h>

#define _(String) gettext(String)

int
main(
  void
  )
{
  setlocale(LC_ALL, "");
  bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
  textdomain(GETTEXT_PACKAGE);

  fputs(_("Hello, world!\n"), stdout);

  return 0;
}
