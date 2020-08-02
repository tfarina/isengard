/*https://stackoverflow.com/questions/45389291/how-to-display-data-from-a-sqlite-database-into-a-gtk-treeview*/

#include "sab_window.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "ab.h"
#include "about_dialog.h"
#include "contact_editor.h"

enum {
  LIST_COL_FIRST_NAME = 0,
  LIST_COL_LAST_NAME,
  LIST_COL_EMAIL,
  LIST_COL_PTR,
};
#define LIST_COL_LAST  LIST_COL_PTR
#define LIST_COL_COUNT (LIST_COL_LAST + 1)

static void sab_window_insert_item(GtkListStore *list_store, ab_contact_t *contact)
{
  GtkTreeIter iter;

  gtk_list_store_append(list_store, &iter);
  gtk_list_store_set(list_store, &iter,
                     LIST_COL_FIRST_NAME, ab_contact_get_first_name(contact),
                     LIST_COL_LAST_NAME, ab_contact_get_last_name(contact),
                     LIST_COL_EMAIL, ab_contact_get_email(contact),
		     LIST_COL_PTR, contact,
                     -1);
}

/* Main Window variables/widgets */
static GtkWidget *main_window;
static GtkWidget *toolbar;
static GtkToolItem *edit_toolbar_button;
static GtkToolItem *delete_toolbar_button;
static GtkWidget *list_view;
static GtkTreeSelection *list_select;
static GtkCellRenderer *renderer;
static GtkTreeViewColumn *column;

static void sab_new_contact_post_cb(ab_contact_t *contact)
{
  GtkListStore *store;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list_view)));

  sab_window_insert_item(store, contact);
}

static void sab_edit_contact_post_cb(ab_contact_t *contact)
{
  GtkTreeModel *model;
  GtkTreeSelection *selection;
  GtkTreeIter iter;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  gtk_tree_selection_get_selected(selection, NULL, &iter);

  gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                     LIST_COL_FIRST_NAME, ab_contact_get_first_name(contact),
                     LIST_COL_LAST_NAME, ab_contact_get_last_name(contact),
                     LIST_COL_EMAIL, ab_contact_get_email(contact),
                     -1);
}

static void sab_window_new_button_cb(GtkWidget *widget, gpointer data)
{
  contact_editor_new(GTK_WINDOW(data), AC_ADD, NULL /*contact*/, sab_new_contact_post_cb);
}

static void sab_window_edit_button_cb(GtkWidget *widget, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeSelection *selection;
  GtkTreeIter iter;
  ab_contact_t *contact;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  gtk_tree_selection_get_selected(selection, NULL, &iter);

  gtk_tree_model_get(model, &iter, LIST_COL_PTR, (ab_contact_t *)&contact, -1);

  contact_editor_new(GTK_WINDOW(data), AC_EDIT, contact, sab_edit_contact_post_cb);
}

static void sab_window_delete_button_cb(GtkWidget *widget, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeSelection *selection;
  GtkTreeIter iter;
  ab_contact_t *contact;
  gboolean has_row = FALSE;
  gint n;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  gtk_tree_selection_get_selected(selection, NULL, &iter);

  gtk_tree_model_get(model, &iter, LIST_COL_PTR, (ab_contact_t *)&contact, -1);

  has_row = gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

  ab_delete_contact(contact);

  /* From claws-mail src/editaddress.c:edit_person_email_delete() */
  if (!has_row) {
    /* The removed row was the last in the list, so iter is not
     * valid. Find out if there is at least one row remaining
     * in the list, and select the last one if so. */
    n = gtk_tree_model_iter_n_children(model, NULL);
    if (n > 0 && gtk_tree_model_iter_nth_child(model, &iter, NULL, n-1)) {
      /* It exists */
      has_row = TRUE;
    }
  }

  if (has_row) {
    gtk_tree_selection_select_iter(selection, &iter);
  }
}

static void sab_window_list_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
{
  gint num_selected;

  num_selected = gtk_tree_selection_count_selected_rows(selection);

  if (num_selected == 1) {
    gtk_widget_set_sensitive(GTK_WIDGET(edit_toolbar_button), TRUE);
  } else {
    gtk_widget_set_sensitive(GTK_WIDGET(edit_toolbar_button), FALSE);
  }

  if (num_selected > 0) {
    gtk_widget_set_sensitive(GTK_WIDGET(delete_toolbar_button), TRUE);
  } else {
    gtk_widget_set_sensitive(GTK_WIDGET(delete_toolbar_button), FALSE);
  }
}

static void toggle_toolbar_cb(GtkWidget *widget, gpointer data)
{
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
    gtk_widget_show(toolbar);
  } else {
    gtk_widget_hide(toolbar);
  }
}

/*
 * Menubar callbacks
 */

static void _on_file_quit_cb(GtkAction *action, gpointer data)
{
  (void)action;
  (void)data;

  gtk_widget_destroy(main_window);
}

static void _on_help_about_cb(GtkWidget *widget, gpointer data)
{
  sab_show_about_dialog();
}

static gint app_delete_event_cb(GtkWidget *widget, GdkEventAny *event,
				 gpointer data)
{
  (void)widget;
  (void)event;
  (void)data;

  return FALSE;
}

/**
 * Handle destroy signal.
 *
 * This function is called when the main application window receives the
 * destroy signal, i.e., it is destroyed.
 */
static void app_destroy_cb(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  gtk_main_quit();
}

#define WINDOW_WIDTH 610
#define WINDOW_HEIGHT 377

void sab_window_new(void)
{
  GtkWidget *vbox;
  GtkWidget *menubar;
  GtkWidget *file_menu;
  GtkWidget *file_item;
  GtkWidget *edit_menu;
  GtkWidget *edit_item;
  GtkWidget *quit_item;
  GtkWidget *new_item;
  GtkWidget *view_menu;
  GtkWidget *view_item;
  GtkWidget *view_tol_item;
  GtkWidget *help_menu;
  GtkWidget *help_item;
  GtkWidget *about_item;
  GtkAccelGroup *accel_group;
  GtkWidget *handlebox;
  GtkWidget* icon;
  GtkToolItem *new_toolbar_button;
  GtkWidget *scrolledwin;

  GtkListStore *list_store; /* Data model */
  alpm_list_t *list, *cur;

  /*
   * Window
   */
  main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(main_window), "Address Book");
  gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(main_window), WINDOW_WIDTH, WINDOW_HEIGHT);

  g_signal_connect(G_OBJECT(main_window), "delete_event",
                   G_CALLBACK(app_delete_event_cb), NULL);
  g_signal_connect(G_OBJECT(main_window), "destroy",
                   G_CALLBACK(app_destroy_cb), NULL);

  vbox = gtk_vbox_new(FALSE, 0);

  gtk_container_add(GTK_CONTAINER(main_window), vbox);

  /*
   * Menubar
   */
  menubar = gtk_menu_bar_new();
  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(main_window), accel_group);

  file_menu = gtk_menu_new();
  file_item = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);

  new_item = gtk_menu_item_new_with_mnemonic("_New Contact");
  g_signal_connect(G_OBJECT(new_item), "activate",
		   G_CALLBACK(sab_window_new_button_cb), main_window);
  gtk_widget_add_accelerator(new_item, "activate", accel_group, GDK_KEY_n,
			     GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), new_item);

  quit_item = gtk_menu_item_new_with_mnemonic("_Quit");
  g_signal_connect(G_OBJECT(quit_item), "activate",
		   G_CALLBACK(_on_file_quit_cb), NULL);
  gtk_widget_add_accelerator(quit_item, "activate", accel_group, GDK_KEY_q,
			     GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);

  edit_menu = gtk_menu_new();
  edit_item = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_item), edit_menu);

  view_menu = gtk_menu_new();
  view_item = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_item), view_menu);

  view_tol_item = gtk_check_menu_item_new_with_mnemonic("Toolbar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_tol_item), TRUE);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_tol_item);

  help_menu = gtk_menu_new();
  help_item = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_item), help_menu);

  about_item = gtk_menu_item_new_with_mnemonic("_About");
  g_signal_connect(G_OBJECT(about_item), "activate",
		   G_CALLBACK(_on_help_about_cb), main_window);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_item);

  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  handlebox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), handlebox, FALSE, FALSE, 0);

  /*
   * Toolbar
   */
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

  icon = gtk_image_new_from_icon_name(GTK_STOCK_NEW, GTK_ICON_SIZE_BUTTON);
  new_toolbar_button = gtk_tool_button_new(icon, "New");
  gtk_widget_set_tooltip_text(GTK_WIDGET(new_toolbar_button), "New contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_toolbar_button, -1);

  icon = gtk_image_new_from_icon_name(GTK_STOCK_EDIT, GTK_ICON_SIZE_BUTTON);
  edit_toolbar_button = gtk_tool_button_new(icon, "Edit");
  gtk_widget_set_tooltip_text(GTK_WIDGET(edit_toolbar_button), "Edit contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), edit_toolbar_button, -1);

  icon = gtk_image_new_from_icon_name(GTK_STOCK_DELETE, GTK_ICON_SIZE_BUTTON);
  delete_toolbar_button = gtk_tool_button_new(icon, "Delete");
  gtk_widget_set_tooltip_text(GTK_WIDGET(delete_toolbar_button), "Delete contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), delete_toolbar_button, -1);

  gtk_widget_set_sensitive(GTK_WIDGET(edit_toolbar_button), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(delete_toolbar_button), FALSE);

  gtk_container_add(GTK_CONTAINER(handlebox), toolbar);
  gtk_container_set_border_width(GTK_CONTAINER(handlebox), 0);

  g_signal_connect(G_OBJECT(new_toolbar_button), "clicked",
		   G_CALLBACK(sab_window_new_button_cb), main_window);

  g_signal_connect(G_OBJECT(edit_toolbar_button), "clicked",
		   G_CALLBACK(sab_window_edit_button_cb), main_window);

  g_signal_connect(G_OBJECT(delete_toolbar_button), "clicked",
		   G_CALLBACK(sab_window_delete_button_cb), NULL);

  g_signal_connect(G_OBJECT(view_tol_item), "activate",
		   G_CALLBACK(toggle_toolbar_cb), toolbar);

  /*
   * List view
   */
  scrolledwin = gtk_scrolled_window_new(NULL, NULL);
  gtk_box_pack_start(GTK_BOX(vbox), scrolledwin, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwin),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  list_store = gtk_list_store_new(LIST_COL_COUNT,
                                  G_TYPE_STRING, /* First name */
                                  G_TYPE_STRING, /* Last name */
                                  G_TYPE_STRING, /* Email */
                                  G_TYPE_POINTER /* Contact pointer */
                                  );
  list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
  g_object_unref(list_store);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list_view), TRUE);

  list_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  g_signal_connect(list_select, "changed",
		   G_CALLBACK(sab_window_list_selection_changed_cb), NULL);

  /* Create the columns. */
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("First Name",
                                                    renderer, "text", LIST_COL_FIRST_NAME, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  column = gtk_tree_view_column_new_with_attributes("Last Name",
                                                    renderer, "text", LIST_COL_LAST_NAME, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  column = gtk_tree_view_column_new_with_attributes("Email",
                                                    renderer, "text", LIST_COL_EMAIL, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  gtk_container_add(GTK_CONTAINER(scrolledwin), list_view);

  gtk_widget_grab_focus(GTK_WIDGET(list_view));

  gtk_widget_show_all(main_window);

  ab_init();

  ab_load_contacts();

  list = ab_get_contact_list();

  for (cur = list; cur; cur = alpm_list_next(cur)) {
    sab_window_insert_item(list_store, (ab_contact_t *)cur->data);
  }
}
