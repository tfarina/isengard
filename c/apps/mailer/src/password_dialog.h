#ifndef _PASSWORD_DIALOG_H_INCLUDED_
#define _PASSWORD_DIALOG_H_INCLUDED_ 1

#include <gtk/gtk.h>

gchar *password_dialog_get(gchar const *server,
			   gchar const *user);

#endif  /* !defined(_PASSWORD_DIALOG_H_INCLUDED_) */
