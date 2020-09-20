#include "about_dialog.h"

#include <gtk/gtk.h>

#define VERSION "0.0.1"
#define PACKAGE_URL "https://sab.com"

#define SAB_LICENSE \
    "This program is free software: you can redistribute it and/or modify it " \
    "under the terms of the GNU General Public License as published by " \
    "the Free Software Foundation; either version 3 of the License, or " \
    "(at your option) any later version."                                \
    "\n\n"                                                               \
    "This program is distributed in the hope that it will be useful, "   \
    "but WITHOUT ANY WARRANTY; without even the implied warranty of "    \
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "     \
    "GNU General Public License for more details."                       \
    "\n\n"                                                               \
    "You should have received a copy of the GNU General Public License " \
    "along with this program. If not, see: https://www.gnu.org/licenses/"

/**
 * Display the About dialog for SAB.
 */
void sab_show_about_dialog(void) {
  GtkWindow *dialog;

  static char const program_name[] = "SAB";
  static char const comments[] = "Simple Address Book";
  static char const copyright[] = "Copyright © 2019";

  dialog = g_object_new(GTK_TYPE_ABOUT_DIALOG,
                        /* Hold the application while the window is shown */
                        "role", "sab-about",
			"window-position", GTK_WIN_POS_CENTER,
			"title", "About SAB",
			"program-name", program_name,
			"comments", comments,
			"copyright", copyright,
			"version", VERSION,
			"license", SAB_LICENSE,
			"wrap-license", TRUE,
			"website", PACKAGE_URL,
			"translator-credits", "CREDITS",
			NULL);

  g_signal_connect(dialog, "response",
                   G_CALLBACK(gtk_widget_destroy), NULL);
  gtk_window_present(dialog);
}
