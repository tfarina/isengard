#include <gtk/gtk.h>
#include <stdio.h>

#define TIMEFORMAT "%I:%M %p"

int main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *label;

  time_t now;
  struct tm *curtime;
  char timebuf[32];

  now = time(NULL);
  curtime = localtime(&now);

  strftime(timebuf, sizeof(timebuf), TIMEFORMAT, curtime);

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Clock");

  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  label = gtk_label_new(timebuf);

  gtk_container_add(GTK_CONTAINER(window), label);

  g_signal_connect(window, "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
