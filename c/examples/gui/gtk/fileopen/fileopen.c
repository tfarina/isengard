#include <gtk/gtk.h>

static
void
on_file_open_cb(GtkMenuItem *menuitem,
		gpointer     user_data)
{
  GtkWidget *dialog;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Open",
				       user_data,
				       GTK_FILE_CHOOSER_ACTION_OPEN,
				       "_Cancel",
				       GTK_RESPONSE_CANCEL,
				       "_Open",
				       GTK_RESPONSE_ACCEPT,
				       NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename = NULL;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    printf("%s\n", filename);
    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}

int
main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *vbox;

  GtkWidget *menubar;
  GtkWidget *file_menu;
  GtkWidget *file_item;
  GtkWidget *open_item;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_title(GTK_WINDOW(window), "Open");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  menubar = gtk_menu_bar_new();

  file_menu = gtk_menu_new();
  file_item = gtk_menu_item_new_with_mnemonic("_File");
  open_item = gtk_menu_item_new_with_mnemonic("_Open...");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);

  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(window), "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(open_item), "activate",
		   G_CALLBACK(on_file_open_cb), window);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
