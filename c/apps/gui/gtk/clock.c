#include <gtk/gtk.h>
#include <stdio.h>

#define TIMEFORMAT "%I:%M:%S %p"

gboolean time_handler(GtkWidget *widget)
{
  time_t now;
  struct tm *curtime;
  char timebuf[32];

  now = time(NULL);
  curtime = localtime(&now);

  strftime(timebuf, sizeof(timebuf), TIMEFORMAT, curtime);

  gtk_label_set_text(GTK_LABEL(widget), timebuf);

  return TRUE;
}

int main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *table;
  GtkWidget *timeframe;
  GtkWidget *label;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Clock");
  gtk_window_set_default_size(GTK_WINDOW(window), 150, 150);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  table = gtk_table_new(1, 1, TRUE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 10);
  gtk_table_set_col_spacings(GTK_TABLE(table), 10);
  gtk_container_add(GTK_CONTAINER(window), table);

  timeframe = gtk_frame_new("Time");
  gtk_frame_set_shadow_type(GTK_FRAME(timeframe), GTK_SHADOW_ETCHED_IN);

  gtk_table_attach_defaults(GTK_TABLE(table), timeframe, 0, 1, 0, 1);

  label = gtk_label_new("");

  gtk_container_add(GTK_CONTAINER(timeframe), label);

  g_signal_connect(window, "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) label);

  gtk_widget_show_all(window);

  time_handler(label);

  gtk_main();

  return 0;
}
