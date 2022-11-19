#include "contact_editor.h"

static action_code_t action_code;
static ab_contact_t *current_contact;
typedef void (*editor_post_cb_t)(ab_contact_t *contact);
static editor_post_cb_t add_edit_post_cb = NULL;

/*
 * Widgets
 */
static GtkWidget *contact_window;
static GtkWidget *fname_entry;
static GtkWidget *lname_entry;
static GtkWidget *email_entry;

static void _contact_editor_ok_cb(GtkWidget *widget, gboolean *cancelled)
{
  char const *first_name;
  char const *last_name;
  char const *email;

  if (action_code == AC_ADD) {
    current_contact = ab_contact_alloc();
  }

  first_name = g_strdup(gtk_entry_get_text(GTK_ENTRY(fname_entry)));
  ab_contact_set_first_name(current_contact, first_name);

  last_name = g_strdup(gtk_entry_get_text(GTK_ENTRY(lname_entry)));
  ab_contact_set_last_name(current_contact, last_name);

  email = g_strdup(gtk_entry_get_text(GTK_ENTRY(email_entry)));
  ab_contact_set_email(current_contact, email);

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

static void _contact_editor_cancel_cb(GtkWidget *widget, gboolean *cancelled)
{
  gtk_widget_destroy(contact_window);
}

void contact_editor_new(GtkWindow *parent, action_code_t ac, ab_contact_t *contact, void (*post_cb)(ab_contact_t *contact))
{
  GtkWidget *vbox;
  GtkWidget *notebook;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *bbox;
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

  notebook = gtk_notebook_new();

  /* Add notebook to vbox. */
  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

  /* Give notebook a border. */
  gtk_container_set_border_width(GTK_CONTAINER(notebook), 6);

  table = gtk_table_new(3, 2, FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(table), 4);
  gtk_table_set_row_spacings(GTK_TABLE(table), 6);
  gtk_table_set_col_spacings(GTK_TABLE(table), 12);

  /* First row. */
  label = gtk_label_new("First Name:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);

  fname_entry = gtk_entry_new();

  gtk_table_attach(GTK_TABLE(table), fname_entry, 1, 2, 0, 1,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  /* Second row. */
  label = gtk_label_new("Last Name:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);

  lname_entry = gtk_entry_new();

  gtk_table_attach(GTK_TABLE(table), lname_entry, 1, 2, 1, 2,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  /* Third row. */
  label = gtk_label_new("Email:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);

  email_entry = gtk_entry_new();

  gtk_table_attach(GTK_TABLE(table), email_entry, 1, 2, 2, 3,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table,
			   gtk_label_new("Name"));

  /*
   * Horizontal Button Box area
   */
  bbox = gtk_hbutton_box_new();
  gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
  gtk_box_set_spacing(GTK_BOX(bbox), 5);
  gtk_container_set_border_width(GTK_CONTAINER(bbox), 4);

  cancel_btn = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
  gtk_box_pack_start(GTK_BOX(bbox), cancel_btn, TRUE, TRUE, 0);

  ok_btn = gtk_button_new_from_stock(GTK_STOCK_OK);
  gtk_box_pack_start(GTK_BOX(bbox), ok_btn, TRUE, TRUE, 0);

  gtk_box_pack_end(GTK_BOX(vbox), bbox, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(ok_btn), "clicked",
                   G_CALLBACK(_contact_editor_ok_cb), NULL);

  g_signal_connect_swapped(cancel_btn, "clicked",
			   G_CALLBACK(_contact_editor_cancel_cb), NULL);

  if (current_contact) {
    gtk_entry_set_text(GTK_ENTRY(fname_entry), ab_contact_get_first_name(current_contact));
    gtk_entry_set_text(GTK_ENTRY(lname_entry), ab_contact_get_last_name(current_contact));
    gtk_entry_set_text(GTK_ENTRY(email_entry), ab_contact_get_email(current_contact));
  }

  gtk_widget_show_all(contact_window);

  /*
   * This should be called after showing the widgets, otherwise the focus
   * won't go to the first name entry.
   */
  gtk_widget_grab_focus(fname_entry);
}
