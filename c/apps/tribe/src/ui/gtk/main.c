#include <gtk/gtk.h>

#include "dirs.h"
#include "util.h"
#include "window.h"

int
main(int argc, char **argv)
{
  GtkWidget *window;

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  gtk_init(&argc, &argv);

  window = addrbook_window_new();

  /* Show main window as late as possible. */
  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
