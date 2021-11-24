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

/*
 * Constants for accessing columns in a GtkListStore.
 */
enum {
  LIST_COL_FIRST_NAME = 0,
  LIST_COL_LAST_NAME,
  LIST_COL_EMAIL,
  LIST_COL_PTR,
};
#define LIST_COL_LAST  LIST_COL_PTR
#define LIST_COL_COUNT (LIST_COL_LAST + 1)

static gint
contact_column_compare_func(GtkTreeModel* model,
			    GtkTreeIter* a,
			    GtkTreeIter* b,
			    gpointer data)
{

  gint sortcol = GPOINTER_TO_INT(data);
  gchar *sa, *sb;
  gint ret = 0;

  switch (sortcol) {
  case LIST_COL_FIRST_NAME:
    gtk_tree_model_get(model, a, LIST_COL_FIRST_NAME, &sa, -1);
    gtk_tree_model_get(model, b, LIST_COL_FIRST_NAME, &sb, -1);
    ret = g_utf8_collate(sa, sb);
    g_free(sa);
    g_free(sb);
    break;
  case LIST_COL_LAST_NAME:
    gtk_tree_model_get(model, a, LIST_COL_LAST_NAME, &sa, -1);
    gtk_tree_model_get(model, b, LIST_COL_LAST_NAME, &sb, -1);
    ret = g_utf8_collate(sa, sb);
    g_free(sa);
    g_free(sb);
    break;
  case LIST_COL_EMAIL:
    gtk_tree_model_get(model, a, LIST_COL_EMAIL, &sa, -1);
    gtk_tree_model_get(model, b, LIST_COL_EMAIL, &sb, -1);
    ret = g_utf8_collate(sa, sb);
    g_free(sa);
    g_free(sb);
    break;
  default:
    g_return_val_if_reached(0);
  }

  return ret;
}

/*
 * Main window widgets.
 */
static GtkWidget *main_window;
static GtkUIManager *ui_manager;
static GtkWidget *toolbar;
static GtkToolItem *tb_edit;
static GtkToolItem *tb_delete;
static GtkListStore *list_store;
static GtkWidget *list_view;
static GtkWidget *statusbar;
static guint statusbar_cid;

static void _on_file_new_cb(GtkAction *action, gpointer data);
static void _on_file_quit_cb(GtkAction *action, gpointer data);
static void _on_edit_select_all_cb(GtkAction *action, gpointer data);
static void _on_edit_edit_cb(GtkAction *action, gpointer data);
static void _on_edit_delete_cb(GtkAction *action, gpointer data);
static void _on_view_toolbar_cb(GtkAction *action, gpointer data);
static void _on_view_statusbar_cb(GtkAction *action, gpointer data);
static void _on_view_fullscreen_cb(GtkAction *action, gpointer data);
static void _on_help_about_cb(GtkAction *action, gpointer data);

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
  {"File/NewContact", GTK_STOCK_NEW, "_New Contact", "<control>N", NULL, G_CALLBACK(_on_file_new_cb) },
  {"File/---", NULL, "---", NULL, NULL, NULL },
  {"File/Quit", GTK_STOCK_QUIT, "_Quit", "<control>Q", NULL, G_CALLBACK(_on_file_quit_cb) },

  /*
   * Edit menu
   */
  {"Edit/SelectAll", NULL, "_Select All", "<control>A", NULL, G_CALLBACK(_on_edit_select_all_cb) },
  {"Edit/---", NULL, "---", NULL, NULL, NULL },
  {"Edit/Edit", GTK_STOCK_EDIT, "_Edit", "<alt>Return", NULL, G_CALLBACK(_on_edit_edit_cb) },
  {"Edit/Delete", GTK_STOCK_DELETE, "_Delete", "<control>D", NULL, G_CALLBACK(_on_edit_delete_cb) },

  /*
   * View menu
   */

  /*
   * Help menu
   */
  {"Help/About", GTK_STOCK_ABOUT, "_About", NULL, NULL, G_CALLBACK(_on_help_about_cb) }
};

static GtkToggleActionEntry menubar_toggle_entries[] =
{
  /*
   * View menu
   */
  {"View/ToolBar", NULL, "Toolbar", "<control>T", NULL, G_CALLBACK(_on_view_toolbar_cb), FALSE },
  {"View/StatusBar", NULL, "Statusbar", NULL, NULL, G_CALLBACK(_on_view_statusbar_cb), FALSE },
  {"View/---", NULL, "---", NULL, NULL, NULL, FALSE },
  {"View/FullScreen", NULL, "_Full Screen", "F11", NULL, G_CALLBACK(_on_view_fullscreen_cb), FALSE },
};

/*
 * Prototype declarations
 */

static void _list_store_append_item(GtkListStore *list_store, ab_contact_t *contact);
static void _on_new_contact_cb(ab_contact_t *contact);
static void _on_edit_contact_cb(ab_contact_t *contact);

static void _edit_selection(gpointer data)
{
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GList *paths;
  GList *cur;
  ab_contact_t *contact;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  paths = gtk_tree_selection_get_selected_rows(selection, &model);

  for (cur = paths; cur; cur = g_list_next(cur))
    {
      if (!gtk_tree_model_get_iter(model, &iter, cur->data))
	{
	  continue;
	}

      gtk_tree_model_get(model, &iter, LIST_COL_PTR, (ab_contact_t *)&contact, -1);
      if (!contact)
	{
	  continue;
	}

      contact_editor_new(GTK_WINDOW(data), AC_EDIT, contact, _on_edit_contact_cb);
    }

  g_list_foreach(paths, (GFunc)gtk_tree_path_free, NULL);
  g_list_free(paths);
}

static void _remove_selection(void)
{
  GtkTreeModel *model;
  GtkTreeSelection *selection;
  GList *paths;
  GList *references = NULL;
  GList *row;
  GtkTreeIter iter;
  ab_contact_t *contact;
  gboolean has_row = FALSE;
  gint n;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  paths = gtk_tree_selection_get_selected_rows(selection, &model);

  /* The code below came mostly from:
   * https://github.com/tristanheaven/gtkhash/blob/master/src/list.c
   * https://github.com/raspberrypi-ui/pcmanfm-bullseye/blob/7393401d8d61d5c1cbd7f653b6549875055bc2e5/src/pref.c#L433
   */

  /* Convert paths to references. */
  for (row = paths; row; row = g_list_next(row))
    {
      GtkTreeRowReference *ref;

      ref = gtk_tree_row_reference_new(model, row->data);
      references = g_list_prepend(references, ref);
      gtk_tree_path_free(row->data);
    }

  g_list_free(paths);

  /* Remove rows from model. */
  for (row = references; row; row = g_list_next(row))
    {
      GtkTreeRowReference *ref = row->data;
      GtkTreePath *path = gtk_tree_row_reference_get_path(ref);

      if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, LIST_COL_PTR, (ab_contact_t *)&contact, -1);

        has_row = gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

        ab_delete_contact(contact);
      }

      gtk_tree_path_free(path);
      gtk_tree_row_reference_free(ref);
    }

  g_list_free(references);

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

static void _on_file_new_cb(GtkAction *action, gpointer data)
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

static void _on_edit_select_all_cb(GtkAction *action, gpointer data)
{
  GtkTreeSelection* selection;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  gtk_tree_selection_select_all(selection);
}

static void _on_edit_edit_cb(GtkAction *action, gpointer data)
{
  _edit_selection(data);
}

static void _on_edit_delete_cb(GtkAction *action, gpointer data)
{
  _remove_selection();
}

/*
 * View menu
 */

static void _on_view_toolbar_cb(GtkAction *action, gpointer data)
{
  gboolean state;

  state = gtk_toggle_action_get_active(GTK_TOGGLE_ACTION(action));

  gtk_widget_set_visible(toolbar, state);
}

static void _on_view_statusbar_cb(GtkAction *action, gpointer data)
{
  gboolean state;

  state = gtk_toggle_action_get_active(GTK_TOGGLE_ACTION(action));

  gtk_widget_set_visible(statusbar, state);
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

static void _on_help_about_cb(GtkAction *action, gpointer data)
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
  _edit_selection(data);
}

static void _on_delete_cb(GtkWidget *widget, gpointer data)
{
  _remove_selection();
}

/*
 * List view callbacks
 */

static void _on_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
{
  gint num_selected;
  GtkWidget *menuitem;
  char buf[256];

  num_selected = gtk_tree_selection_count_selected_rows(selection);

  menuitem = gtk_ui_manager_get_widget(ui_manager, "/Menu/Edit/Edit");
  if (num_selected == 1) {
    gtk_widget_set_sensitive(menuitem, TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(tb_edit), TRUE);
  } else {
    gtk_widget_set_sensitive(menuitem, FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(tb_edit), FALSE);
  }

  menuitem = gtk_ui_manager_get_widget(ui_manager, "/Menu/Edit/Delete");
  if (num_selected > 0) {
    gtk_widget_set_sensitive(menuitem, TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(tb_delete), TRUE);
  } else {
    gtk_widget_set_sensitive(menuitem, FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(tb_delete), FALSE);
  }

  if (statusbar != NULL) {
    gtk_statusbar_pop(GTK_STATUSBAR(statusbar), statusbar_cid);
    if (num_selected > 1) {
      snprintf(buf, sizeof(buf), "%d items selected", num_selected);
    } else {
      snprintf(buf, sizeof(buf), "%d item selected", num_selected);
    }
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), statusbar_cid, buf);
  }
}

static gboolean _on_list_button_pressed_cb(GtkTreeView *widget,
                                           GdkEventButton *event,
                                           gpointer data)
{
  GtkTreePath *path;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  ab_contact_t *contact;

  if (event->window != gtk_tree_view_get_bin_window(widget)) {
    return FALSE;
  }

  /* Figure out which node was clicked. */
  if (!gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(list_view), event->x, event->y, &path, &column, NULL, NULL)) {
    return FALSE;
  }
  if (column == gtk_tree_view_get_column(GTK_TREE_VIEW(list_view), 0)) {
    gtk_tree_path_free(path);
    return FALSE;
  }

  gtk_tree_model_get_iter(GTK_TREE_MODEL(list_store), &iter, path);
  gtk_tree_path_free(path);
  gtk_tree_model_get(GTK_TREE_MODEL(list_store), &iter, LIST_COL_PTR, &contact, -1);

  if (event->button == 1 && event->type == GDK_2BUTTON_PRESS) {
    if (contact != NULL) {
      contact_editor_new(GTK_WINDOW(main_window), AC_EDIT, contact, _on_edit_contact_cb);
      return TRUE;
    }
  }

  return FALSE;
}

static gboolean
_on_list_key_press_cb(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  if (event && event->keyval == GDK_KEY_Delete)
    {
      _remove_selection ();
      return TRUE;
    }

  return FALSE;
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
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GList *paths;
  GList *cur;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  paths = gtk_tree_selection_get_selected_rows(selection, &model);

  for (cur = paths; cur; cur = g_list_next(cur))
    {
      if (gtk_tree_model_get_iter(model, &iter, cur->data))
	{
	  gtk_list_store_set(GTK_LIST_STORE(model), &iter,
			     LIST_COL_FIRST_NAME, ab_contact_get_first_name(contact),
			     LIST_COL_LAST_NAME, ab_contact_get_last_name(contact),
			     LIST_COL_EMAIL, ab_contact_get_email(contact),
			     -1);
	}

      gtk_tree_path_free(cur->data);
    }

  g_list_free(paths);
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
			"/Menu/Edit", "Select All", "Edit/SelectAll", GTK_UI_MANAGER_MENUITEM, FALSE);
  gtk_ui_manager_add_ui(ui_manager, gtk_ui_manager_new_merge_id(ui_manager),
			"/Menu/Edit", "Separator1", "Edit/---", GTK_UI_MANAGER_SEPARATOR, FALSE);
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

  menuitem = gtk_ui_manager_get_widget(ui_manager, "/Menu/Edit/Edit");
  gtk_widget_set_sensitive(menuitem, FALSE);

  menuitem = gtk_ui_manager_get_widget(ui_manager, "/Menu/Edit/Delete");
  gtk_widget_set_sensitive(menuitem, FALSE);

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

static GtkWidget *_list_view_create(void)
{
  GtkWidget *scrolledwin;
  GtkTreeSortable *sortable;
  GtkTreeSelection *selection;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  /* Setup the scrolled window. */
  scrolledwin = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwin),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  /* Create the list store. */
  list_store = gtk_list_store_new(LIST_COL_COUNT,
                                  G_TYPE_STRING,    /* First name */
                                  G_TYPE_STRING,    /* Last name */
                                  G_TYPE_STRING,    /* Email */
                                  G_TYPE_POINTER);  /* Contact pointer */

  sortable = GTK_TREE_SORTABLE(list_store);
  gtk_tree_sortable_set_sort_func(sortable, LIST_COL_FIRST_NAME,
				  contact_column_compare_func,
				  GINT_TO_POINTER(LIST_COL_FIRST_NAME), NULL);
  gtk_tree_sortable_set_sort_func(sortable, LIST_COL_LAST_NAME,
				  contact_column_compare_func,
				  GINT_TO_POINTER(LIST_COL_LAST_NAME), NULL);
  gtk_tree_sortable_set_sort_func(sortable, LIST_COL_EMAIL,
				  contact_column_compare_func,
				  GINT_TO_POINTER(LIST_COL_EMAIL), NULL);

  /* Create the list view. */
  list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
  g_object_unref(list_store);

  /* Set selection properties. */
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);
  g_signal_connect(selection, "changed",
		   G_CALLBACK(_on_selection_changed_cb), NULL);

  /* Handle double-clicking. */
  g_signal_connect(list_view, "button_press_event",
		   G_CALLBACK(_on_list_button_pressed_cb), NULL);
  /* Handle key press. */
  g_signal_connect(list_view, "key_press_event",
		   G_CALLBACK(_on_list_key_press_cb), NULL);

  /* Create the columns. */
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("First Name",
                                                    renderer, "text", LIST_COL_FIRST_NAME, NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_column_set_sort_column_id(column, LIST_COL_FIRST_NAME);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Last Name",
                                                    renderer, "text", LIST_COL_LAST_NAME, NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_column_set_sort_column_id(column, LIST_COL_LAST_NAME);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Email",
                                                    renderer, "text", LIST_COL_EMAIL, NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_column_set_sort_column_id(column, LIST_COL_EMAIL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  /* Attach list view the scrolled window. */
  gtk_container_add(GTK_CONTAINER(scrolledwin), list_view);

  return scrolledwin;
}

static void _populate_list_view(GtkListStore* list_store)
{
  char *dbdir;
  alpm_list_t *list, *cur;

  dbdir = dirs_get_user_data_dir();

  ab_init(dbdir);

  ab_load_contacts();

  list = ab_get_contact_list();

  for (cur = list; cur; cur = alpm_list_next(cur)) {
    _list_store_append_item(list_store, (ab_contact_t *)cur->data);
  }
}

GtkWidget *addrbook_window_new(void)
{
  GtkWidget *vbox;
  GtkWidget *menubar;
  GtkWidget *menuitem;
  GtkWidget *scrolledwin;

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
  scrolledwin = _list_view_create();
  gtk_box_pack_start(GTK_BOX(vbox), scrolledwin, TRUE, TRUE, 0);

  /*
   * Statusbar
   */
  statusbar = gtk_statusbar_new();
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 0);

  statusbar_cid = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "Address Book Window");

  /*
   * Set up menu items
   */
  menuitem = gtk_ui_manager_get_widget(ui_manager, "/Menu/View/ToolBar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
  menuitem = gtk_ui_manager_get_widget(ui_manager, "/Menu/View/StatusBar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);

  gtk_widget_grab_focus(list_view);

  _populate_list_view(list_store);

  return main_window;
}
