/*https://stackoverflow.com/questions/45389291/how-to-display-data-from-a-sqlite-database-into-a-gtk-treeview*/

#include <gtk/gtk.h>

#include "user.h"

enum {
  LIST_FIRST_NAME,
  LIST_LAST_NAME,
  LIST_EMAIL,
  LIST_COL_PTR,
  LIST_NUM_COLUMNS
};

static void insert_item(GtkWidget *list_view, user_t *user)
{
  GtkTreeModel *model = NULL;
  GtkTreeIter iter;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));

  gtk_list_store_append(GTK_LIST_STORE(model), &iter);
  gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                     LIST_FIRST_NAME, user->fname,
                     LIST_LAST_NAME, user->lname,
                     LIST_EMAIL, user->email,
		     LIST_COL_PTR, user,
                     -1);
}

int main(int argc, char** argv)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *list_view;
  GtkListStore *list_store;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  alpm_list_t *users, *i;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Address Book");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

  vbox = gtk_vbox_new(FALSE, 0);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  list_store = gtk_list_store_new(LIST_NUM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);
  list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
  g_object_unref(list_store);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list_view), TRUE);

  /* Create the columns. */
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("First Name",
                                                    renderer, "text", LIST_FIRST_NAME, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Last Name",
                                                    renderer, "text", LIST_LAST_NAME, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Email",
                                                    renderer, "text", LIST_EMAIL, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  gtk_box_pack_start(GTK_BOX(vbox), list_view, TRUE, TRUE, 0);

  users = user_get_records();

  for (i = users; i; i = alpm_list_next(i)) {
    insert_item(list_view, (user_t *)i->data);
  }

  g_signal_connect(G_OBJECT(window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

