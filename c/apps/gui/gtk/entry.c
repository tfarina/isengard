#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *fname_entry;
  GtkWidget *lname_entry;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Create");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  g_signal_connect(window, "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  table = gtk_table_new(3, 2, FALSE);
  gtk_container_add(GTK_CONTAINER(window), table);
  gtk_container_set_border_width(GTK_CONTAINER(table), 4 );
  gtk_table_set_row_spacings(GTK_TABLE(table), 4);
  gtk_table_set_col_spacings(GTK_TABLE(table), 4);

  fname_entry = gtk_entry_new();
  lname_entry = gtk_entry_new();

  /* First row. */
  label = gtk_label_new("First Name");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

  gtk_table_attach(GTK_TABLE(table), fname_entry, 1, 2, 0, 1,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  /* Second row. */
  label = gtk_label_new("Last Name");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

  gtk_table_attach(GTK_TABLE(table), lname_entry, 1, 2, 1, 2,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
