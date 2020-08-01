#include <gtk/gtk.h>

#include "sab_window.h"

int main(int argc, char** argv)
{
  gtk_init(&argc, &argv);

  sab_window_new();

  gtk_main();

  return 0;
}
