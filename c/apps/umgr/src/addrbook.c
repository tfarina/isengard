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

GtkWidget *window = NULL;
GtkToolItem *edit_item = NULL;
GtkToolItem *delete_item = NULL;

static void new_item_cb(GtkWidget *widget, gpointer data)
{
  GtkWidget *new_window;

  new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Create New Person");
  gtk_window_set_position(GTK_WINDOW(new_window), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(new_window), TRUE);
  gtk_widget_show(new_window);
}

static void list_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
{
  gint num_selected;

  num_selected = gtk_tree_selection_count_selected_rows(selection);

  if (num_selected == 1) {
    gtk_widget_set_sensitive(GTK_WIDGET(edit_item), TRUE);
  } else {
    gtk_widget_set_sensitive(GTK_WIDGET(edit_item), FALSE);
  }

  if (num_selected > 0) {
    gtk_widget_set_sensitive(GTK_WIDGET(delete_item), TRUE);
  } else {
    gtk_widget_set_sensitive(GTK_WIDGET(delete_item), FALSE);
  }
}

int main(int argc, char** argv)
{
  GtkWidget *vbox;
  GtkWidget *handlebox;
  GtkWidget *toolbar;
  GtkToolItem *new_item;
  GtkWidget *scrolledwin;
  GtkWidget *list_view;
  GtkTreeSelection *list_select;
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

  handlebox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), handlebox, FALSE, FALSE, 0);

  toolbar = gtk_toolbar_new();
  new_item = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_item, -1);

  edit_item = gtk_tool_button_new_from_stock(GTK_STOCK_EDIT);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), edit_item, -1);

  delete_item = gtk_tool_button_new_from_stock(GTK_STOCK_DELETE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), delete_item, -1);

  gtk_widget_set_sensitive(GTK_WIDGET(edit_item), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(delete_item), FALSE);

  gtk_container_add(GTK_CONTAINER(handlebox), toolbar);
  gtk_container_set_border_width(GTK_CONTAINER(handlebox), 0);

  g_signal_connect(G_OBJECT(new_item), "clicked",
		   G_CALLBACK(new_item_cb), NULL);

  list_store = gtk_list_store_new(LIST_NUM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);
  list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
  g_object_unref(list_store);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list_view), TRUE);

  list_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  g_signal_connect(list_select, "changed",
		   G_CALLBACK(list_selection_changed_cb), NULL);

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

  scrolledwin = gtk_scrolled_window_new(NULL, NULL);
  gtk_box_pack_start(GTK_BOX(vbox), scrolledwin, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwin),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  gtk_container_add(GTK_CONTAINER(scrolledwin), list_view);

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

