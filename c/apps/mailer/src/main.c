#include <gtk/gtk.h>

#include "compose.h"

int
main(int argc, char **argv)
{
  ComposeWindow *compose;

  gtk_init(&argc, &argv);

  compose = compose_window_new();
  if (compose == 0) {
    return 1;
  }

  gtk_main();
  compose_window_delete(compose);

  return 0;
}
