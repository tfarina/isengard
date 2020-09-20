#include "about_dialog.h"

#include <gtk/gtk.h>

#define VERSION "0.0.1"
#define PACKAGE_URL "https://sab.com"

#define SAB_ABBR "SAB"
#define SAB_NAME "Simple Address Book"
#define SAB_COPYRIGHT "Copyright © 2019"
#define SAB_LICENSE \
    "SAB is free software: you can redistribute it and/or modify it "   \
        "under the terms of the GNU General Public License as published by " \
        "the Free Software Foundation; either version 3 of the License, or " \
        "(at your option) any later version."                                \
        "\n\n"                                                               \
        "SAB is distributed in the hope that it will be useful, "           \
        "but WITHOUT ANY WARRANTY; without even the implied warranty of "    \
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "     \
        "GNU General Public License for more details."                       \
        "\n\n"                                                               \
        "You should have received a copy of the GNU General Public License " \
    "along with SAB. If not, see: https://www.gnu.org/licenses/"

/**
 * Display the About dialog for SAB.
 */
void sab_show_about_dialog(void) {
  GtkWindow *dialog;

  dialog = g_object_new(GTK_TYPE_ABOUT_DIALOG,
                        /* Hold the application while the window is shown */
                        "role", "sab-about",
			"window-position", GTK_WIN_POS_CENTER,
			"title", "About SAB",
			"program-name", SAB_ABBR,
			"copyright", SAB_COPYRIGHT,
			"comments", SAB_NAME,
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
