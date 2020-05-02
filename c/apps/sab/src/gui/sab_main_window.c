/*https://stackoverflow.com/questions/45389291/how-to-display-data-from-a-sqlite-database-into-a-gtk-treeview*/

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "ab.h"
#include "about_dialog.h"

enum {
  LIST_COL_FIRST_NAME = 0,
  LIST_COL_LAST_NAME,
  LIST_COL_EMAIL,
  LIST_COL_PTR,
  LIST_COL_NUM
};

typedef enum action_code_e {
  AC_ADD = 0,
  AC_EDIT,
} action_code_t;

static void sab_main_window_insert_item(GtkListStore *list_store, ab_contact_t *contact)
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

/* Editor dialog variables/widgets */
static GtkWidget *contact_window;
static GtkWidget *fname_entry;
static GtkWidget *lname_entry;
static GtkWidget *email_entry;
static action_code_t action_code;
static ab_contact_t *current_contact;
typedef void (*editor_post_cb_t)(ab_contact_t *contact);
static editor_post_cb_t add_edit_post_cb = NULL;

static void sab_contact_window_ok_btn_cb(GtkWidget *widget, gboolean *cancelled)
{
  char const *name;

  if (action_code == AC_ADD) {
    current_contact = ab_contact_alloc();
  }

  name = g_strdup(gtk_entry_get_text(GTK_ENTRY(fname_entry)));
  ab_contact_set_first_name(current_contact, name);

  name = g_strdup(gtk_entry_get_text(GTK_ENTRY(lname_entry)));
  ab_contact_set_last_name(current_contact, name);

  name = g_strdup(gtk_entry_get_text(GTK_ENTRY(email_entry)));
  ab_contact_set_email(current_contact, name);

  if (action_code == AC_ADD) {
    ab_add_contact(current_contact);
  } else {
    ab_change_contact(current_contact);
  }

  gtk_widget_destroy(contact_window);

  if (add_edit_post_cb) {
    add_edit_post_cb(current_contact);
  }
}

static void sab_contact_window_cancel_btn_cb(GtkWidget *widget, gboolean *cancelled)
{
  gtk_widget_destroy(contact_window);
}

static void sab_contact_window(GtkWindow *parent, action_code_t ac, ab_contact_t *contact, void (*post_cb)(ab_contact_t *contact))
{
  GtkWidget *vbox;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *confirm_area;
  GtkWidget *cancel_btn;
  GtkWidget *ok_btn;

  char const *title;

  action_code = ac;
  current_contact = contact;
  add_edit_post_cb = post_cb;

  if (current_contact) {
    title = "Edit Person";
  } else {
    title = "Create New Person";
  }

  contact_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(contact_window), title);
  gtk_window_set_transient_for(GTK_WINDOW(contact_window), parent);
  gtk_window_set_position(GTK_WINDOW(contact_window), GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_modal(GTK_WINDOW(contact_window), TRUE);
  gtk_container_set_border_width(GTK_CONTAINER(contact_window), 6);

  vbox = gtk_vbox_new(FALSE, 6);
  gtk_container_add(GTK_CONTAINER(contact_window), vbox);

  table = gtk_table_new(3, 2, FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(table), 4);
  gtk_table_set_row_spacings(GTK_TABLE(table), 4);
  gtk_table_set_col_spacings(GTK_TABLE(table), 4);
  gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, FALSE, 0);

  fname_entry = gtk_entry_new();
  lname_entry = gtk_entry_new();
  email_entry = gtk_entry_new();

  /* First row. */
  label = gtk_label_new("First Name");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

  gtk_table_attach(GTK_TABLE(table), fname_entry, 1, 2, 0, 1,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  /* Second row. */
  label = gtk_label_new("Last Name");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

  gtk_table_attach(GTK_TABLE(table), lname_entry, 1, 2, 1, 2,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  /* Third row. */
  label = gtk_label_new("Email");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

  gtk_table_attach(GTK_TABLE(table), email_entry, 1, 2, 2, 3,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  confirm_area = gtk_hbutton_box_new();
  gtk_button_box_set_layout(GTK_BUTTON_BOX(confirm_area), GTK_BUTTONBOX_END);
  gtk_box_set_spacing(GTK_BOX(confirm_area), 5);
  gtk_container_set_border_width(GTK_CONTAINER(confirm_area), 4);

  cancel_btn = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
  gtk_box_pack_start(GTK_BOX(confirm_area), cancel_btn, TRUE, TRUE, 0);

  ok_btn = gtk_button_new_from_stock(GTK_STOCK_OK);
  gtk_box_pack_start(GTK_BOX(confirm_area), ok_btn, TRUE, TRUE, 0);

  gtk_box_pack_end(GTK_BOX(vbox), confirm_area, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(ok_btn), "clicked",
                   G_CALLBACK(sab_contact_window_ok_btn_cb), NULL);

  g_signal_connect_swapped(cancel_btn, "clicked",
			   G_CALLBACK(sab_contact_window_cancel_btn_cb), NULL);

  if (current_contact) {
    gtk_entry_set_text(GTK_ENTRY(fname_entry), ab_contact_get_first_name(current_contact));
    gtk_entry_set_text(GTK_ENTRY(lname_entry), ab_contact_get_last_name(current_contact));
    gtk_entry_set_text(GTK_ENTRY(email_entry), ab_contact_get_email(current_contact));
  }

  gtk_widget_grab_focus(fname_entry);
  gtk_widget_show_all(contact_window);
}

static void sab_new_contact_post_cb(ab_contact_t *contact)
{
  GtkListStore *store;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list_view)));

  sab_main_window_insert_item(store, contact);
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

static void sab_main_window_new_button_cb(GtkWidget *widget, gpointer data)
{
  sab_contact_window(GTK_WINDOW(data), AC_ADD, NULL /*contact*/, sab_new_contact_post_cb);
}

static void sab_main_window_edit_button_cb(GtkWidget *widget, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeSelection *selection;
  GtkTreeIter iter;
  ab_contact_t *contact;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));

  gtk_tree_selection_get_selected(selection, NULL, &iter);

  gtk_tree_model_get(model, &iter, LIST_COL_PTR, (ab_contact_t *)&contact, -1);

  sab_contact_window(GTK_WINDOW(data), AC_EDIT, contact, sab_edit_contact_post_cb);
}

static void sab_main_window_delete_button_cb(GtkWidget *widget, gpointer data)
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

static void sab_main_window_list_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
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

static void app_quit_cb(GtkAction *action, gpointer data)
{
  (void)action;
  (void)data;

  gtk_widget_destroy(main_window);
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

static void sab_main_window_about_cb(GtkWidget *widget, gpointer data)
{
  sab_about_dialog_show();
}

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

int main(int argc, char** argv)
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

  gtk_init(&argc, &argv);

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

  menubar = gtk_menu_bar_new();
  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(main_window), accel_group);

  file_menu = gtk_menu_new();
  file_item = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);

  quit_item = gtk_menu_item_new_with_mnemonic("_Quit");
  g_signal_connect(G_OBJECT(quit_item), "activate",
		   G_CALLBACK(app_quit_cb), NULL);
  gtk_widget_add_accelerator(quit_item, "activate", accel_group, GDK_KEY_q,
			     GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);

  edit_menu = gtk_menu_new();
  edit_item = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_item), edit_menu);

  new_item = gtk_menu_item_new_with_mnemonic("_New Contact");
  g_signal_connect(G_OBJECT(new_item), "activate",
		   G_CALLBACK(sab_main_window_new_button_cb), main_window);
  gtk_widget_add_accelerator(new_item, "activate", accel_group, GDK_KEY_n,
			     GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), new_item);

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
		   G_CALLBACK(sab_main_window_about_cb), main_window);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_item);

  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  handlebox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), handlebox, FALSE, FALSE, 0);

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
		   G_CALLBACK(sab_main_window_new_button_cb), main_window);

  g_signal_connect(G_OBJECT(edit_toolbar_button), "clicked",
		   G_CALLBACK(sab_main_window_edit_button_cb), main_window);

  g_signal_connect(G_OBJECT(delete_toolbar_button), "clicked",
		   G_CALLBACK(sab_main_window_delete_button_cb), NULL);

  g_signal_connect(G_OBJECT(view_tol_item), "activate",
		   G_CALLBACK(toggle_toolbar_cb), toolbar);

  scrolledwin = gtk_scrolled_window_new(NULL, NULL);
  gtk_box_pack_start(GTK_BOX(vbox), scrolledwin, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwin),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  list_store = gtk_list_store_new(LIST_COL_NUM,
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
		   G_CALLBACK(sab_main_window_list_selection_changed_cb), NULL);

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

  list = ab_get_contact_list();

  for (cur = list; cur; cur = alpm_list_next(cur)) {
    sab_main_window_insert_item(list_store, (ab_contact_t *)cur->data);
  }

  gtk_main();

  return 0;
}
