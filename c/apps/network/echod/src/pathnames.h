/*
 * pathnames.h - define default paths for echod
 */
#ifndef PATHNAMES_H
#define PATHNAMES_H

/*
 * Paths are controlled via cmake. One can pass the following options:
 *
 * cmake -DCMAKE_INSTALL_PREFIX=/usr -DSYSCONFDIR=/etc -DLOCALSTATEDIR=/var
 */

#define DEF_CFGFILE SYSCONFDIR "/echod.conf"

#define DEF_PIDDIR LOCALSTATEDIR "/run/echod"
#define DEF_PIDFILE DEF_PIDDIR "/echod.pid"

#endif /* PATHNAMES_H */
