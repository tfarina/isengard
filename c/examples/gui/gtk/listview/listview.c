#include <gtk/gtk.h>

enum {
  LIST_FRUITS_COLUMN = 0,
  LIST_NUM_COLUMNS
};

static void append_fruit_to_model(GtkWidget *list, const gchar *str)
{
  GtkTreeModel *model;
  GtkTreeIter iter;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));

  gtk_list_store_append(GTK_LIST_STORE(model), &iter);
  gtk_list_store_set(GTK_LIST_STORE(model), &iter, LIST_FRUITS_COLUMN, str, -1);
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *list;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;
  char const *fruits[] = { "Apple", "Banana", "Blackberry", "Coconut", "Kiwi" };
  guint i;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "List view");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

  g_signal_connect(G_OBJECT (window), "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  list = gtk_tree_view_new();

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes("Fruits", renderer, "text",
						    LIST_FRUITS_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  store = gtk_list_store_new(LIST_NUM_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

  g_object_unref(store);

  gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);

  for (i = 0; i < G_N_ELEMENTS(fruits); i++)
  {
    append_fruit_to_model(list, fruits[i]);
  }

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

