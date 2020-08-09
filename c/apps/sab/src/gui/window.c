/*https://stackoverflow.com/questions/45389291/how-to-display-data-from-a-sqlite-database-into-a-gtk-treeview*/

#include "window.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "ab.h"
#include "about_dialog.h"
#include "contact_editor.h"

#define WINDOW_WIDTH 610
#define WINDOW_HEIGHT 377

enum {
  LIST_COL_FIRST_NAME = 0,
  LIST_COL_LAST_NAME,
  LIST_COL_EMAIL,
  LIST_COL_PTR,
};
#define LIST_COL_LAST  LIST_COL_PTR
#define LIST_COL_COUNT (LIST_COL_LAST + 1)

/*
 * Widgets
 */
static GtkWidget *main_window;
static GtkWidget *toolbar;
static GtkToolItem *tb_edit;
static GtkToolItem *tb_delete;
static GtkWidget *list_view;
static GtkWidget *statusbar;

/*
 * Prototype declarations
 */

static void _list_store_append_item(GtkListStore *list_store, ab_contact_t *contact);
static void _on_new_contact_cb(ab_contact_t *contact);
static void _on_edit_contact_cb(ab_contact_t *contact);

/*
 * Window callbacks
 */

static gint _on_delete_event_cb(GtkWidget *widget, GdkEventAny *event,
				gpointer data)
{
  (void)widget;
  (void)event;
  (void)data;

  return FALSE;
}

static void _on_destroy_cb(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  gtk_main_quit();
}

/*
 * Menubar callbacks
 */

/* File menu */

static void _on_file_new_cb(GtkWidget *widget, gpointer data)
{
  contact_editor_new(GTK_WINDOW(data), AC_ADD, NULL /*contact*/, _on_new_contact_cb);
}

static void _on_file_quit_cb(GtkAction *action, gpointer data)
{
  (void)action;
  (void)data;

  gtk_widget_destroy(main_window);
}

/* Edit menu */

static void _on_edit_select_all_cb(GtkWidget *widget, gpointer data)
{
}

/* View menu */

static void _on_view_toolbar_cb(GtkWidget *widget, gpointer data)
{
  gboolean state;

  state = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  /*
   * This check is necessary to fix the following error:
   * (sab_gui:14142): Gtk-CRITICAL **: IA__gtk_widget_set_visible: assertion 'GTK_IS_WIDGET (widget)' failed
   *
   * That occurs because the toolbar is create after the menubar, but this callback is called
   * before the toolbar is created.
   */
  if (toolbar != NULL) {
    gtk_widget_set_visible(toolbar, state);
  }
}

static void _on_view_statusbar_cb(GtkWidget *widget, gpointer data)
{
  gboolean state;

  state = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  /*
   * This check is necessary to avoid the following error:
   * (sab_gui:14142): Gtk-CRITICAL **: IA__gtk_widget_set_visible: assertion 'GTK_IS_WIDGET (widget)' failed
   *
   * That occurs because the menubar is created before the statusbar, and this callback gets called
   * before the statusbar is created.
   */
  if (statusbar != NULL) {
    gtk_widget_set_visible(statusbar, state);
  }
}

static void _on_view_fullscreen_cb(GtkWidget *widget, gpointer data)
{
  if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
    gtk_window_unfullscreen(GTK_WINDOW(main_window));
  } else {
    gtk_window_fullscreen(GTK_WINDOW(main_window));
  }
}

/* Help menu */

static void _on_help_about_cb(GtkWidget *widget, gpointer data)
{
  sab_show_about_dialog();
}

/*
 * Toolbar callbacks
 */

static void _on_new_cb(GtkWidget *widget, gpointer data)
{
  contact_editor_new(GTK_WINDOW(data), AC_ADD, NULL /*contact*/, _on_new_contact_cb);
}

static void _on_edit_cb(GtkWidget *widget, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeSelection *selection;
  GtkTreeIter iter;
  ab_contact_t *contact;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  gtk_tree_selection_get_selected(selection, NULL, &iter);

  gtk_tree_model_get(model, &iter, LIST_COL_PTR, (ab_contact_t *)&contact, -1);

  contact_editor_new(GTK_WINDOW(data), AC_EDIT, contact, _on_edit_contact_cb);
}

static void _on_delete_cb(GtkWidget *widget, gpointer data)
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

/*
 * List view callbacks
 */

static void _on_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
{
  gint num_selected;

  num_selected = gtk_tree_selection_count_selected_rows(selection);

  if (num_selected == 1) {
    gtk_widget_set_sensitive(GTK_WIDGET(tb_edit), TRUE);
  } else {
    gtk_widget_set_sensitive(GTK_WIDGET(tb_edit), FALSE);
  }

  if (num_selected > 0) {
    gtk_widget_set_sensitive(GTK_WIDGET(tb_delete), TRUE);
  } else {
    gtk_widget_set_sensitive(GTK_WIDGET(tb_delete), FALSE);
  }
}

/*
 * Contact editor callbacks
 */

static void _on_new_contact_cb(ab_contact_t *contact)
{
  GtkListStore *store;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list_view)));

  _list_store_append_item(store, contact);
}

static void _on_edit_contact_cb(ab_contact_t *contact)
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

/*
 * Helper functions
 */

static void _list_store_append_item(GtkListStore *list_store, ab_contact_t *contact)
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

static void _file_menu_create(GtkMenuShell *menu, GtkAccelGroup *accel)
{
  GtkWidget *menuitem;

  menuitem = gtk_menu_item_new_with_mnemonic("_New Contact");
  g_signal_connect(G_OBJECT(menuitem), "activate",
		   G_CALLBACK(_on_file_new_cb), main_window);
  gtk_widget_add_accelerator(menuitem, "activate", accel, GDK_KEY_n,
			     GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_menu_shell_append(menu, menuitem);

  menuitem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(menu, menuitem);

  menuitem = gtk_menu_item_new_with_mnemonic("_Quit");
  g_signal_connect(G_OBJECT(menuitem), "activate",
		   G_CALLBACK(_on_file_quit_cb), NULL);
  gtk_widget_add_accelerator(menuitem, "activate", accel, GDK_KEY_q,
			     GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_menu_shell_append(menu, menuitem);
}

static void _edit_menu_create(GtkMenuShell *menu, GtkAccelGroup *accel)
{
  GtkWidget *menuitem;

  menuitem = gtk_menu_item_new_with_mnemonic("Select _All");
  g_signal_connect(G_OBJECT(menuitem), "activate",
		   G_CALLBACK(_on_edit_select_all_cb), main_window);
  gtk_widget_add_accelerator(menuitem, "activate", accel, GDK_KEY_A,
			     GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_menu_shell_append(menu, menuitem);
}

static void _view_menu_create(GtkMenuShell *menu, GtkAccelGroup *accel)
{
  GtkWidget *menuitem;

  menuitem = gtk_check_menu_item_new_with_mnemonic("Toolbar");
  g_signal_connect(G_OBJECT(menuitem), "activate",
		   G_CALLBACK(_on_view_toolbar_cb), toolbar);
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
  gtk_menu_shell_append(menu, menuitem);

  menuitem = gtk_check_menu_item_new_with_mnemonic("Statusbar");
  g_signal_connect(G_OBJECT(menuitem), "activate",
		   G_CALLBACK(_on_view_statusbar_cb), statusbar);
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
  gtk_menu_shell_append(menu, menuitem);

  menuitem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(menu, menuitem);

  menuitem = gtk_check_menu_item_new_with_mnemonic("_Fullscreen");
  g_signal_connect(G_OBJECT(menuitem), "activate",
		   G_CALLBACK(_on_view_fullscreen_cb), main_window);
  gtk_widget_add_accelerator(menuitem, "activate", accel, GDK_F11,
			     GDK_MODE_DISABLED, GTK_ACCEL_VISIBLE);
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), FALSE);
  gtk_menu_shell_append(menu, menuitem);
}

static void _help_menu_create(GtkMenuShell *menu, GtkAccelGroup *accel)
{
  GtkWidget *menuitem;

  menuitem = gtk_menu_item_new_with_mnemonic("_About");
  g_signal_connect(G_OBJECT(menuitem), "activate",
		   G_CALLBACK(_on_help_about_cb), main_window);
  gtk_menu_shell_append(menu, menuitem);
}

static GtkWidget *_menubar_create(GtkAccelGroup *accel)
{
  GtkWidget *menubar;
  GtkWidget *submenu;
  GtkWidget *menuitem;

  menubar = gtk_menu_bar_new();

  /* File menu */
  submenu = gtk_menu_new();
  menuitem = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
  _file_menu_create(GTK_MENU_SHELL(submenu), accel);

  /* Edit menu */
  submenu = gtk_menu_new();
  menuitem = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
  _edit_menu_create(GTK_MENU_SHELL(submenu), accel);

  /* View menu */
  submenu = gtk_menu_new();
  menuitem = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
  _view_menu_create(GTK_MENU_SHELL(submenu), accel);

  /* Help menu */
  submenu = gtk_menu_new();
  menuitem = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
  _help_menu_create(GTK_MENU_SHELL(submenu), accel);

  return menubar;
}

void addrbook_window_new(void)
{
  GtkAccelGroup *accel_group;
  GtkWidget *vbox;
  GtkWidget *menubar;
  GtkWidget* icon;
  GtkToolItem *tb_new;
  GtkWidget *scrolledwin;
  GtkTreeSelection *list_select;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
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
                   G_CALLBACK(_on_delete_event_cb), NULL);
  g_signal_connect(G_OBJECT(main_window), "destroy",
                   G_CALLBACK(_on_destroy_cb), NULL);

  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(main_window), accel_group);

  /*
   * Vertical box
   */
  vbox = gtk_vbox_new(FALSE, 0);

  /*
   * Menubar
   */
  menubar = _menubar_create(accel_group);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);

  /*
   * Toolbar
   */
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

  /* New button */
  icon = gtk_image_new_from_icon_name(GTK_STOCK_NEW, GTK_ICON_SIZE_BUTTON);
  tb_new = gtk_tool_button_new(icon, "New");
  gtk_widget_set_tooltip_text(GTK_WIDGET(tb_new), "New contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb_new, -1);

  /* Edit button */
  icon = gtk_image_new_from_icon_name(GTK_STOCK_EDIT, GTK_ICON_SIZE_BUTTON);
  tb_edit = gtk_tool_button_new(icon, "Edit");
  gtk_widget_set_tooltip_text(GTK_WIDGET(tb_edit), "Edit contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb_edit, -1);

  /* Delete button */
  icon = gtk_image_new_from_icon_name(GTK_STOCK_DELETE, GTK_ICON_SIZE_BUTTON);
  tb_delete = gtk_tool_button_new(icon, "Delete");
  gtk_widget_set_tooltip_text(GTK_WIDGET(tb_delete), "Delete contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb_delete, -1);

  gtk_widget_set_sensitive(GTK_WIDGET(tb_edit), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(tb_delete), FALSE);

  g_signal_connect(G_OBJECT(tb_new), "clicked",
		   G_CALLBACK(_on_new_cb), main_window);

  g_signal_connect(G_OBJECT(tb_edit), "clicked",
		   G_CALLBACK(_on_edit_cb), main_window);

  g_signal_connect(G_OBJECT(tb_delete), "clicked",
		   G_CALLBACK(_on_delete_cb), NULL);

  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);

  /*
   * List view
   */
  scrolledwin = gtk_scrolled_window_new(NULL, NULL);
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
		   G_CALLBACK(_on_selection_changed_cb), NULL);

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
  gtk_box_pack_start(GTK_BOX(vbox), scrolledwin, TRUE, TRUE, 0);

  /*
   * Statusbar
   */
  statusbar = gtk_statusbar_new();
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(main_window), vbox);

  gtk_widget_grab_focus(GTK_WIDGET(list_view));

  gtk_widget_show_all(main_window);

  ab_init();

  ab_load_contacts();

  list = ab_get_contact_list();

  for (cur = list; cur; cur = alpm_list_next(cur)) {
    _list_store_append_item(list_store, (ab_contact_t *)cur->data);
  }
}
