#include <gtk/gtk.h>

enum {
  COLUMN = 0,
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
  GtkTreeIter toplevel, child;
  GtkTreeModel *treemodel;

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

  column = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(column, "Folders");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(column, renderer, TRUE);
  gtk_tree_view_column_add_attribute(column, renderer, "text", COLUMN);

  treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);

  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel, COLUMN, "Local Folders", -1);

  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child, COLUMN, "Inbox", -1);

  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child, COLUMN, "Outbox", -1);

  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child, COLUMN, "Sent Items", -1);

  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child, COLUMN, "Deleted Items", -1);

  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child, COLUMN, "Drafts", -1);

  treemodel = GTK_TREE_MODEL(treestore);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), treemodel);
  g_object_unref(treemodel);

  gtk_box_pack_start(GTK_BOX(vbox), treeview, TRUE, TRUE, 1);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
