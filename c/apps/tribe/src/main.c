#include <gtk/gtk.h>

#include "ab.h"
#include "dirs.h"
#include "util.h"
#include "window.h"

int
main(int argc, char **argv)
{
  int rc;
  int status = 0; /* success */
  char *dbdir;
  GtkWidget *window;

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  rc = ab_init(dbdir);
  if (rc < 0) {
    status = 1;
    goto out;
  }

  gtk_init(&argc, &argv);

  window = create_main_window();

  /* Show main window as late as possible. */
  gtk_widget_show_all(window);

  gtk_main();

out:
  ab_fini();

  return status;
}
