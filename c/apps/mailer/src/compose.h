#ifndef _COMPOSE_H_INCLUDED_
#define _COMPOSE_H_INCLUDED_ 1

typedef struct _ComposeWindow ComposeWindow;

ComposeWindow *compose_window_new(void);
void compose_window_delete(ComposeWindow *compose);

#endif  /* !defined(_COMPOSE_H_INCLUDED_) */
