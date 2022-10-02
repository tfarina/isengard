#include "password_dialog.h"

static GtkWidget *dialog;

static gboolean ack, fin;

static void
cancel_btn_clicked_cb(GtkWidget *widget, gpointer data)
{
  ack = FALSE;
  fin = TRUE;
}

static void
ok_btn_clicked_cb(GtkWidget *widget, gpointer data)
{
  ack = TRUE;
  fin = TRUE;
}

/*
 * This code was borrowed from claws-mail:
 *
 * https://git.claws-mail.org/?p=claws.git;a=blob;f=src/gtk/inputdialog.h;h=426077254b9ec76990c49b91a50d027bbcf5cd2d;hb=HEAD
 *
 */

gchar *
password_dialog_get(gchar const *server,
		    gchar const *user)
{
  gchar *message;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *icon_p;
  GtkWidget *msg_label;
  GtkWidget *entry;
  GtkWidget *confirm_area;
  GtkWidget *cancel_btn;
  GtkWidget *ok_btn;
  gchar *pass;

  message = g_strdup_printf("Enter password for %s:", user);

  dialog = gtk_dialog_new();

  gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 375, 100);
  gtk_window_set_title(GTK_WINDOW(dialog), "Input password");

  vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_box_set_spacing(GTK_BOX(vbox), 14);
  hbox = gtk_hbox_new(FALSE, 12);
  gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
  gtk_widget_show(hbox);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  /*
   * Icon
   */
  icon_p = gtk_image_new_from_stock(GTK_STOCK_DIALOG_AUTHENTICATION,
				    GTK_ICON_SIZE_DIALOG); 
  gtk_misc_set_alignment(GTK_MISC(icon_p), 0.5, 0.0);
  gtk_box_pack_start(GTK_BOX(hbox), icon_p, FALSE, FALSE, 0);

  vbox = gtk_vbox_new(FALSE, 12);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 0);
  gtk_widget_show(vbox);

  /*
   * Message Label
   */
  msg_label = gtk_label_new(message);
  gtk_misc_set_alignment(GTK_MISC(msg_label), 0, 0.5);
  gtk_label_set_justify(GTK_LABEL(msg_label), GTK_JUSTIFY_LEFT);
  gtk_box_pack_start(GTK_BOX(vbox), msg_label, FALSE, FALSE, 0);
  gtk_widget_show(msg_label);

  /*
   * Password entry
   */
  entry = gtk_entry_new();
  gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
  gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

  /*
   * Confirmation Area
   */
  confirm_area = gtk_hbutton_box_new();
  gtk_button_box_set_layout(GTK_BUTTON_BOX(confirm_area), GTK_BUTTONBOX_END);
  gtk_box_set_spacing(GTK_BOX(confirm_area), 5);

  /*
   * Cancel Button
   */
  cancel_btn = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
  gtk_widget_set_can_default(cancel_btn, TRUE);
  gtk_box_pack_start(GTK_BOX(confirm_area), cancel_btn, TRUE, TRUE, 0);
  gtk_widget_show(cancel_btn);

  /*
   * OK Button
   */
  ok_btn = gtk_button_new_from_stock(GTK_STOCK_OK);
  gtk_widget_set_can_default(ok_btn, TRUE);
  gtk_box_pack_start(GTK_BOX(confirm_area), ok_btn, TRUE, TRUE, 0);
  gtk_widget_show(ok_btn);

  g_signal_connect(G_OBJECT(cancel_btn), "clicked",
		   G_CALLBACK(cancel_btn_clicked_cb), NULL);
  g_signal_connect(G_OBJECT(ok_btn), "clicked",
		   G_CALLBACK(ok_btn_clicked_cb), NULL);

  gtk_box_pack_end(GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG(dialog))),
		   confirm_area, FALSE, FALSE, 0);

  gtk_container_set_border_width(GTK_CONTAINER(confirm_area), 5);

  gtk_widget_show_all(gtk_dialog_get_content_area(GTK_DIALOG(dialog)));

  gtk_widget_grab_default(ok_btn);

  gtk_window_present(GTK_WINDOW(dialog));

  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

  ack = fin = FALSE;

  while (fin == FALSE) {
    gtk_main_iteration();
  }

  if (ack) {
    pass = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
  }

  gtk_widget_destroy(dialog);
  dialog = NULL;

  return pass;
}
