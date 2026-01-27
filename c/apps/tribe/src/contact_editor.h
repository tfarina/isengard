#ifndef _CONTACT_EDITOR_H_INCLUDED_
#define _CONTACT_EDITOR_H_INCLUDED_

#include <gtk/gtk.h>

#include "ab.h"

typedef enum action_code_e {
  AC_ADD = 0,
  AC_EDIT,
} action_code_t;

typedef void (*editor_post_cb_t)(ab_contact_t *contact);

void contact_editor_new(GtkWindow *parent, action_code_t ac, ab_contact_t *contact, editor_post_cb_t post_cb);

#endif  /* !defined(_CONTACT_EDITOR_H_INCLUDED_) */
