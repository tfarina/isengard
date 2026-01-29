#ifndef PASSWORD_DIALOG_H
#define PASSWORD_DIALOG_H

#include <gtk/gtk.h>

gchar *password_dialog_get(gchar const *server,
			   gchar const *user);

#endif /* PASSWORD_DIALOG_H */
