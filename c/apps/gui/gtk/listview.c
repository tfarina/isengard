#include <gtk/gtk.h>

enum {
  LIST_FRUITS_COLUMN = 0,
  LIST_NUM_COLUMNS
};

static void list_add_item(GtkWidget *list, const gchar *str)
{
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_FRUITS_COLUMN, str, -1);
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *list;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  GtkWidget *vbox;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  list = gtk_tree_view_new();

  gtk_window_set_title(GTK_WINDOW(window), "List view");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

  vbox = gtk_vbox_new(FALSE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), TRUE);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes("Fruits", renderer, "text",
						    LIST_FRUITS_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  store = gtk_list_store_new(LIST_NUM_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

  g_object_unref(store);

  list_add_item(list, "Apple");
  list_add_item(list, "Banana");
  list_add_item(list, "Blackberry");
  list_add_item(list, "Coconut");
  list_add_item(list, "Kiwi");

  g_signal_connect(G_OBJECT (window), "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
