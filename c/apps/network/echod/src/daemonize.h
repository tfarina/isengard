#ifndef _DAEMONIZE_H_INCLUDED_
#define _DAEMONIZE_H_INCLUDED_ 1

/*
 * http://man7.org/linux/man-pages/man3/daemon.3.html
 * https://github.com/twitter/twemcache/blob/7c846a16e8eb2930b35c7484720b8ef59125c277/src/mc.c
 * https://github.com/yandex/odyssey/blob/b8d65d7225774c90f519f7de77911704cedbd7f5/sources/daemon.h
 * https://github.com/Netflix/dynomite/blob/032a6f3dc4523e55beebbfff04d993ad60a43dec/src/dynomite.c
 */

/**
 * Put the program in the background and redirect stdin, stdout and stderr
 * to /dev/null.
 *
 * @return 0 on success. If an error occurs, returns -1.
 */
int daemonize(void);

#endif  /* !defined(_DAEMONIZE_H_INCLUDED_) */
