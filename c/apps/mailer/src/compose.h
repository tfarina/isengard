#ifndef COMPOSE_H
#define COMPOSE_H

typedef struct _ComposeWindow ComposeWindow;

ComposeWindow *compose_window_new(void);
void compose_window_delete(ComposeWindow *compose);

#endif /* COMPOSE_H */
