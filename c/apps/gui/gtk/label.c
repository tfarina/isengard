#include <gtk/gtk.h>

int main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *label;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Widget - Label");

  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  label = gtk_label_new("Hello, world!");

  gtk_container_add(GTK_CONTAINER(window), label);

  g_signal_connect(window, "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
