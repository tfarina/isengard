#include "contact_editor.h"

/* Editor dialog variables/widgets */
static GtkWidget *contact_window;
static GtkWidget *fname_entry;
static GtkWidget *lname_entry;
static GtkWidget *email_entry;
static action_code_t action_code;
static ab_contact_t *current_contact;
typedef void (*editor_post_cb_t)(ab_contact_t *contact);
static editor_post_cb_t add_edit_post_cb = NULL;

static void _contact_editor_ok_cb(GtkWidget *widget, gboolean *cancelled)
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

static void _contact_editor_cancel_cb(GtkWidget *widget, gboolean *cancelled)
{
  gtk_widget_destroy(contact_window);
}

void contact_editor_new(GtkWindow *parent, action_code_t ac, ab_contact_t *contact, void (*post_cb)(ab_contact_t *contact))
{
  GtkWidget *vbox;
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

  gtk_widget_grab_focus(fname_entry);
  gtk_widget_show_all(contact_window);
}
