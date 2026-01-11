#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *vbox;
  
  GtkWidget *toolbar;
  GtkToolItem *new_item;
  GtkToolItem *open_item;
  GtkToolItem *save_item;
  GtkToolItem *separator_item;
  GtkToolItem *exit_item;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_title(GTK_WINDOW(window), "Tool bar");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

  new_item = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_item, -1);

  open_item = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open_item, -1);

  save_item = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_item, -1);

  separator_item = gtk_separator_tool_item_new();
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator_item, -1); 

  exit_item = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit_item, -1);

  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 5);

  g_signal_connect(G_OBJECT(exit_item), "clicked", 
		   G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(window), "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
