#include <gtk/gtk.h>

#include "window.h"

int main(int argc, char** argv)
{
  gtk_init(&argc, &argv);

  addrbook_window_new();

  gtk_main();

  return 0;
}
