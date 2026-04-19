#include <gtk/gtk.h>

enum
{
  COLUMN_NAME = 0,
  NUM_COLS
};

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *treeview;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  GtkTreeStore *treestore;
  GtkTreeIter parent_iter;
  GtkTreeIter node_iter;
  char const *special_folders[] = { "Inbox", "Outbox", "Sent Items", "Deleted Items", "Drafts" };
  guint i;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Tree View");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_widget_set_size_request(window, 350, 300);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  g_signal_connect(G_OBJECT(window), "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  treeview = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);

  column = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(column, "Folders");

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(column, renderer, TRUE);
  gtk_tree_view_column_add_attribute(column, renderer, "text", COLUMN_NAME);

  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(treestore));
  g_object_unref(treestore);

  gtk_box_pack_start(GTK_BOX(vbox), treeview, TRUE, TRUE, 0);

  gtk_tree_store_append(treestore, &parent_iter, NULL);
  gtk_tree_store_set(treestore, &parent_iter, COLUMN_NAME, "Local Folders", -1);

  for (i = 0; i < G_N_ELEMENTS(special_folders); i++) {
    gtk_tree_store_append(treestore, &node_iter, &parent_iter);
    gtk_tree_store_set(treestore, &node_iter, COLUMN_NAME, special_folders[i], -1);
  }

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
