#ifndef ED_DAEMON_H_
#define ED_DAEMON_H_

/*
http://man7.org/linux/man-pages/man3/daemon.3.html
https://github.com/twitter/twemcache/blob/7c846a16e8eb2930b35c7484720b8ef59125c277/src/mc.c
https://github.com/yandex/odyssey/blob/b8d65d7225774c90f519f7de77911704cedbd7f5/sources/daemon.h
https://github.com/Netflix/dynomite/blob/032a6f3dc4523e55beebbfff04d993ad60a43dec/src/dynomite.c
*/

/**
 * Daemonize and redirect stdin, stdout and stderr to /dev/null.
 *
 * @param[in] dump_core If set to 0 then dynomite tries to chdir to /.
 *
 * @return 0 on success or -1 on error.
 */
int ed_daemon_detach(int dump_core);

#endif  /* ED_DAEMON_H_ */
