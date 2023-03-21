/*
 * Example from: http://zetcode.com/gui/gtk2/gtkwidgets/
 */

#include <gtk/gtk.h>

int main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *table;

  GtkWidget *timeframe;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 250, 250);
  gtk_window_set_title(GTK_WINDOW(window), "Time Frame");

  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  table = gtk_table_new(1, 1, TRUE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 10);
  gtk_table_set_col_spacings(GTK_TABLE(table), 10);
  gtk_container_add(GTK_CONTAINER(window), table);

  timeframe = gtk_frame_new("Time");
  gtk_frame_set_shadow_type(GTK_FRAME(timeframe), GTK_SHADOW_ETCHED_IN);

  gtk_table_attach_defaults(GTK_TABLE(table), timeframe, 0, 1, 0, 1);

  g_signal_connect(G_OBJECT(window), "destroy",
		   G_CALLBACK(gtk_main_quit), G_OBJECT(window));

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
