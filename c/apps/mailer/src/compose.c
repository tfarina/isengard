#include "compose.h"

#include <stdlib.h>
#include <stdio.h>

#include <gtk/gtk.h>

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

static void _compose_send_cb(gpointer data)
{
  ComposeWindow *compose = data;
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
  GtkWidget *from_hbox;
  GtkWidget *to_hbox;
  GtkWidget *subject_hbox;
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
  gtk_window_set_title(GTK_WINDOW(compose->window), "Compose");
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
   * From
   */
  from_hbox = gtk_hbox_new(FALSE, 0);

  label = gtk_label_new("From: ");
  gtk_box_pack_start(GTK_BOX(from_hbox), label, FALSE, FALSE, 0);

  compose->from_entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(from_hbox), compose->from_entry, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), from_hbox, FALSE, FALSE, 0);

  /*
   * To
   */
  to_hbox = gtk_hbox_new(FALSE, 0);

  label = gtk_label_new("To: ");
  gtk_box_pack_start(GTK_BOX(to_hbox), label, FALSE, FALSE, 0);

  compose->to_entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(to_hbox), compose->to_entry, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), to_hbox, FALSE, FALSE, 0);

  /*
   * Subject
   */
  subject_hbox = gtk_hbox_new(FALSE, 0);

  label = gtk_label_new("Subject: ");
  gtk_box_pack_start(GTK_BOX(subject_hbox), label, FALSE, FALSE, 0);

  compose->subject_entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(subject_hbox), compose->subject_entry, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), subject_hbox, FALSE, FALSE, 0);

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
