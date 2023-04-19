#include "compose.h"

#include <stdlib.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include "password_dialog.h"

#define WINDOW_WIDTH 610
#define WINDOW_HEIGHT 377

struct _ComposeWindow
{
  GtkWidget *window;
  GtkWidget *from_entry;
  GtkWidget *to_entry;
  GtkWidget *subject_entry;
  GtkWidget *textview;
  GtkWidget *statusbar;
};

static void _compose_delete(ComposeWindow *compose)
{
  gtk_main_quit();
}

static gboolean _compose_delete_event_cb(gpointer data)
{
  ComposeWindow *compose = data;

  _compose_delete(compose);

  return TRUE;
}

static void show_error(GtkWidget *window, gchar const *message)
{
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				  GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
				  GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
				  "%s", "Error");

  gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
					   "%s", message);

  gtk_dialog_run(GTK_DIALOG(dialog));

  gtk_widget_destroy(dialog);
}

static gboolean dialog_confirm(GtkWidget *window, gchar const *primary_text, gchar const *message)
{
  GtkWidget *dialog;
  gint response;

  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				  GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
				  GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
				  "%s", primary_text);

  gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
					   "%s", message);

  response = gtk_dialog_run(GTK_DIALOG(dialog));

  gtk_widget_destroy(dialog);

  return response == GTK_RESPONSE_YES;
}

static void _compose_send_cb(gpointer data)
{
  ComposeWindow *compose = data;
  gchar *pass;
  gchar *from;
  gchar *to;
  gchar const *subject;
  GtkTextBuffer *buffer;
  GtkTextIter start;
  GtkTextIter end;
  gchar *text = NULL;

  /*
   * Header
   */
  from = gtk_editable_get_chars(GTK_EDITABLE(compose->from_entry), 0, -1);
  to = gtk_editable_get_chars(GTK_EDITABLE(compose->to_entry), 0, -1);
  subject = gtk_entry_get_text(GTK_ENTRY(compose->subject_entry));

  /*
   * Body
   */
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(compose->textview));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  /*
   * Check entries
   */
  if (!from || *from == '\0') {
    show_error(compose->window, "Please, specify the return address (From).");
    return;
  }
  if (!to || *to == '\0') {
    show_error(compose->window, "The recipient is empty.");
    return;
  }

  if (!subject || *subject == '\0') {
    if (!dialog_confirm(compose->window, "Send",
			"Subject is empty. Send it anyway?")) {
      return;
    }
  }

  pass = password_dialog_get("", from);

  printf("%s\n", pass);
  printf("%s\n", from);
  printf("%s\n", to);
  printf("%s\n", subject);
  printf("%s\n", text);
  
  g_free(text);
  _compose_delete(compose);
}

ComposeWindow *compose_window_new(void)
{
  ComposeWindow *compose;
  GtkWidget *vbox;
  GtkWidget *toolbar;
  GtkWidget *icon;
  GtkToolItem *send_button;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *scrolledwin;

  compose = malloc(sizeof(*compose));
  if (compose == 0) {
    return 0;
  }

  /*
   * Window
   */
  compose->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(compose->window), "Write New Message - Mailer");
  gtk_window_set_position(GTK_WINDOW(compose->window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(compose->window), WINDOW_WIDTH, WINDOW_HEIGHT);

  g_signal_connect(G_OBJECT(compose->window), "delete_event",
                   G_CALLBACK(_compose_delete_event_cb), compose);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(compose->window), vbox);

  /*
   * Toolbar
   */
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

  icon = gtk_image_new_from_icon_name(GTK_STOCK_EXECUTE, GTK_ICON_SIZE_BUTTON);
  send_button = gtk_tool_button_new(icon, "Send");
  g_signal_connect_swapped(G_OBJECT(send_button), "clicked",
			   G_CALLBACK(_compose_send_cb), compose);
  gtk_widget_set_tooltip_text(GTK_WIDGET(send_button), "Send message");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), send_button, -1);

  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);

  /*
   * Table
   */
  table = gtk_table_new(3, 2, FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(table), 4);
  gtk_table_set_row_spacings(GTK_TABLE(table), 6);
  gtk_table_set_col_spacings(GTK_TABLE(table), 12);
  gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, FALSE, 0);

  /*
   * First row - From
   */
  label = gtk_label_new("From:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);

  compose->from_entry = gtk_entry_new();

  gtk_table_attach(GTK_TABLE(table), compose->from_entry, 1, 2, 0, 1,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  /*
   * Second row - To
   */
  label = gtk_label_new("To:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);

  compose->to_entry = gtk_entry_new();

  gtk_table_attach(GTK_TABLE(table), compose->to_entry, 1, 2, 1, 2,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  /*
   * Third row - Subject
   */
  label = gtk_label_new("Subject:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3,
		   GTK_FILL, 0, 0, 0);
  gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);

  compose->subject_entry = gtk_entry_new();

  gtk_table_attach(GTK_TABLE(table), compose->subject_entry, 1, 2, 2, 3,
		   GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0);

  /*
   * Text view
   */
  scrolledwin = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwin),
				 GTK_POLICY_AUTOMATIC,
				 GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwin),
				      GTK_SHADOW_IN);
  gtk_box_pack_start(GTK_BOX(vbox), scrolledwin, TRUE, TRUE, 0);

  compose->textview = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(compose->textview),
			      GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(compose->textview), TRUE);

  gtk_container_add(GTK_CONTAINER(scrolledwin), compose->textview);

  /*
   * Statusbar
   */
  compose->statusbar = gtk_statusbar_new();
  gtk_box_pack_start(GTK_BOX(vbox), compose->statusbar, FALSE, TRUE, 0);

  /*
   * Display
   */
  gtk_widget_grab_focus(compose->from_entry);
  gtk_widget_show_all(vbox);
  gtk_widget_show(compose->window);

  return compose;
}

void compose_window_delete(ComposeWindow *compose)
{
  gtk_widget_destroy(compose->window);
  free(compose);
}
