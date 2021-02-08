/*https://stackoverflow.com/questions/45389291/how-to-display-data-from-a-sqlite-database-into-a-gtk-treeview*/

#include "window.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "ab.h"
#include "about_dialog.h"
#include "contact_editor.h"
#include "dirs.h"

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

static void _on_file_new_cb(GtkWidget *widget, gpointer data);
static void _on_file_quit_cb(GtkAction *action, gpointer data);
static void _on_edit_edit_cb(GtkWidget *widget, gpointer data);
static void _on_edit_delete_cb(GtkWidget *widget, gpointer data);
static void _on_view_toolbar_cb(GtkAction *action, gpointer data);
static void _on_view_statusbar_cb(GtkAction *action, gpointer data);
static void _on_view_fullscreen_cb(GtkAction *action, gpointer data);
static void _on_help_about_cb(GtkWidget *widget, gpointer data);

static GtkActionEntry menubar_entries[] =
{
  {"Menu", NULL, "Menu", NULL, NULL, NULL },

  /*
   * Menus
   */
  {"File", NULL, "_File", NULL, NULL, NULL },
  {"Edit", NULL, "_Edit", NULL, NULL, NULL },
  {"View", NULL, "_View", NULL, NULL, NULL },
  {"Help", NULL, "_Help", NULL, NULL, NULL },

  /*
   * File menu
   */
  {"File/NewContact", NULL, "_New Contact", "<control>N", NULL, G_CALLBACK(_on_file_new_cb) },
  {"File/---", NULL, "---", NULL, NULL, NULL },
  {"File/Quit", NULL, "_Quit", "<control>Q", NULL, G_CALLBACK(_on_file_quit_cb) },

  /*
   * Edit menu
   */
  {"Edit/Edit", NULL, "_Edit", "<control>Return", NULL, G_CALLBACK(_on_edit_edit_cb) },
  {"Edit/Delete", NULL, "_Delete", "<control>D", NULL, G_CALLBACK(_on_edit_delete_cb) },

  /*
   * View menu
   */

  /*
   * Help menu
   */
  {"Help/About", NULL, "_About", NULL, NULL, G_CALLBACK(_on_help_about_cb) }
};

static GtkToggleActionEntry menubar_toggle_entries[] =
{
  /*
   * View menu
   */
  {"View/ToolBar", NULL, "Toolbar", NULL, NULL, G_CALLBACK(_on_view_toolbar_cb), FALSE },
  {"View/StatusBar", NULL, "Statusbar", NULL, NULL, G_CALLBACK(_on_view_statusbar_cb), FALSE },
  {"View/---", NULL, "---", NULL, NULL, NULL, FALSE },
  {"View/FullScreen", NULL, "_Fullscreen", "F11", NULL, G_CALLBACK(_on_view_fullscreen_cb), FALSE },
};

/*
 * Prototype declarations
 */

static void _list_store_append_item(GtkListStore *list_store, ab_contact_t *contact);
static void _on_new_contact_cb(ab_contact_t *contact);
static void _on_edit_contact_cb(ab_contact_t *contact);

/*
 * Window callbacks
 */

static gboolean _on_delete_event_cb(GtkWidget *widget,
				    GdkEvent  *event,
				    gpointer   data)
{
  (void)widget;
  (void)event;
  (void)data;

  gtk_main_quit();

  return TRUE;
}

/*
 * Menubar callbacks
 */

/*
 * File menu
 */

static void _on_file_new_cb(GtkWidget *widget, gpointer data)
{
  contact_editor_new(GTK_WINDOW(data), AC_ADD, NULL /*contact*/, _on_new_contact_cb);
}

static void _on_file_quit_cb(GtkAction *action, gpointer data)
{
  (void)action;
  (void)data;

  gtk_main_quit();
}

/*
 * Edit menu
 */

static void _on_edit_edit_cb(GtkWidget *widget, gpointer data)
{
}

static void _on_edit_delete_cb(GtkWidget *widget, gpointer data)
{
}

/*
 * View menu
 */

static void _on_view_toolbar_cb(GtkAction *action, gpointer data)
{
  gboolean state;

  state = gtk_toggle_action_get_active(GTK_TOGGLE_ACTION(action));

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

static void _on_view_statusbar_cb(GtkAction *action, gpointer data)
{
  gboolean state;

  state = gtk_toggle_action_get_active(GTK_TOGGLE_ACTION(action));

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

static void _on_view_fullscreen_cb(GtkAction *action, gpointer data)
{
  if (!gtk_toggle_action_get_active(GTK_TOGGLE_ACTION(action))) {
    gtk_window_unfullscreen(GTK_WINDOW(main_window));
  } else {
    gtk_window_fullscreen(GTK_WINDOW(main_window));
  }
}

/*
 * Help menu
 */

static void _on_help_about_cb(GtkWidget *widget, gpointer data)
{
  show_about_dialog(GTK_WINDOW(main_window));
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

static GtkWidget *_menubar_create(void)
{
  GtkUIManager *ui_manager;
  GtkActionGroup *action_group;

  GtkWidget *menubar;
  GtkWidget *menuitem;

  ui_manager = gtk_ui_manager_new();

  action_group = gtk_action_group_new("Menu");
  gtk_action_group_set_translation_domain(action_group, NULL);
  gtk_action_group_add_actions(action_group, menubar_entries,
			       G_N_ELEMENTS(menubar_entries), NULL);
  gtk_action_group_add_toggle_actions(action_group, menubar_toggle_entries,
			              G_N_ELEMENTS(menubar_toggle_entries), NULL);
  gtk_ui_manager_insert_action_group(ui_manager, action_group, 0);

  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/", "Menu", NULL, GTK_UI_MANAGER_MENUBAR, FALSE);

  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu", "File", "File", GTK_UI_MANAGER_MENU, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu", "Edit", "Edit", GTK_UI_MANAGER_MENU, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu", "View", "View", GTK_UI_MANAGER_MENU, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu", "Help", "Help", GTK_UI_MANAGER_MENU, FALSE);

  /* File menu */
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/File", "NewContact", "File/NewContact", GTK_UI_MANAGER_MENUITEM, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/File", "Separator1", "File/---", GTK_UI_MANAGER_SEPARATOR, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/File", "Quit", "File/Quit", GTK_UI_MANAGER_MENUITEM, FALSE);

  /* Edit menu */
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/Edit", "Edit", "Edit/Edit", GTK_UI_MANAGER_MENUITEM, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/Edit", "Delete", "Edit/Delete", GTK_UI_MANAGER_MENUITEM, FALSE);

  /* View menu */
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/View", "ToolBar", "View/ToolBar", GTK_UI_MANAGER_MENUITEM, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/View", "StatusBar", "View/StatusBar", GTK_UI_MANAGER_MENUITEM, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/View", "Separator1", "View/---", GTK_UI_MANAGER_SEPARATOR, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/View", "FullScreen", "View/FullScreen", GTK_UI_MANAGER_MENUITEM, FALSE);

  /* Help menu */
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/Help", "About", "Help/About", GTK_UI_MANAGER_MENUITEM, FALSE);

  gtk_window_add_accel_group(GTK_WINDOW(main_window), gtk_ui_manager_get_accel_group(ui_manager));

  menubar = gtk_ui_manager_get_widget(ui_manager, "/Menu");

  menuitem = gtk_ui_manager_get_widget(ui_manager, "/Menu/View/ToolBar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
  menuitem = gtk_ui_manager_get_widget(ui_manager, "/Menu/View/StatusBar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);

  return menubar;
}

static GtkWidget *_toolbar_create(void)
{
  GtkWidget* icon;
  GtkToolItem *tb_new;

  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

  /* New button */
  icon = gtk_image_new_from_icon_name(GTK_STOCK_NEW, GTK_ICON_SIZE_BUTTON);
  tb_new = gtk_tool_button_new(icon, "New");
  gtk_widget_set_tooltip_text(GTK_WIDGET(tb_new), "New contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb_new, -1);
  g_signal_connect(G_OBJECT(tb_new), "clicked",
		   G_CALLBACK(_on_new_cb), main_window);

  /* Edit button */
  icon = gtk_image_new_from_icon_name(GTK_STOCK_EDIT, GTK_ICON_SIZE_BUTTON);
  tb_edit = gtk_tool_button_new(icon, "Edit");
  gtk_widget_set_tooltip_text(GTK_WIDGET(tb_edit), "Edit contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb_edit, -1);
  g_signal_connect(G_OBJECT(tb_edit), "clicked",
		   G_CALLBACK(_on_edit_cb), main_window);

  /* Delete button */
  icon = gtk_image_new_from_icon_name(GTK_STOCK_DELETE, GTK_ICON_SIZE_BUTTON);
  tb_delete = gtk_tool_button_new(icon, "Delete");
  gtk_widget_set_tooltip_text(GTK_WIDGET(tb_delete), "Delete contact");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tb_delete, -1);
  g_signal_connect(G_OBJECT(tb_delete), "clicked",
		   G_CALLBACK(_on_delete_cb), NULL);

  gtk_widget_set_sensitive(GTK_WIDGET(tb_edit), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(tb_delete), FALSE);

  return toolbar;
}

void addrbook_window_new(void)
{
  char *dbdir;
  GtkWidget *vbox;
  GtkWidget *menubar;
  GtkWidget *scrolledwin;
  GtkTreeSelection *selection;
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

  /*
   * Vertical box
   */
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(main_window), vbox);

  /*
   * Menubar
   */
  menubar = _menubar_create();
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);

  /*
   * Toolbar
   */
  toolbar = _toolbar_create();
  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);

  /*
   * List view
   */
  scrolledwin = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwin),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  list_store = gtk_list_store_new(LIST_COL_COUNT,
                                  G_TYPE_STRING,    /* First name */
                                  G_TYPE_STRING,    /* Last name */
                                  G_TYPE_STRING,    /* Email */
                                  G_TYPE_POINTER);  /* Contact pointer */
  list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
  g_object_unref(list_store);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  g_signal_connect(selection, "changed",
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

  gtk_widget_grab_focus(GTK_WIDGET(list_view));

  gtk_widget_show_all(main_window);

  dbdir = dirs_get_user_data_dir();

  ab_init(dbdir);

  ab_load_contacts();

  list = ab_get_contact_list();

  for (cur = list; cur; cur = alpm_list_next(cur)) {
    _list_store_append_item(list_store, (ab_contact_t *)cur->data);
  }
}
