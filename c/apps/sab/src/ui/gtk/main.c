#include <gtk/gtk.h>

#include "dirs.h"
#include "util.h"
#include "window.h"

int
main(int argc, char **argv)
{
  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  gtk_init(&argc, &argv);

  addrbook_window_new();

  gtk_main();

  return 0;
}
