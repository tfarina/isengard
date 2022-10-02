/*
 * pathnames.h - define default paths for echod
 */
#ifndef _PATHNAMES_H_INCLUDED_
#define _PATHNAMES_H_INCLUDED_ 1

/*
 * Paths are controlled via cmake. One can pass the following options:
 *
 * cmake -DCMAKE_INSTALL_PREFIX=/usr -DSYSCONFDIR=/etc -DLOCALSTATEDIR=/var
 */

#define DEF_CFGFILE SYSCONFDIR "/echod.conf"

#define DEF_PIDDIR LOCALSTATEDIR "/run/echod"
#define DEF_PIDFILE DEF_PIDDIR "/echod.pid"

#endif  /* !defined(_PATHNAMES_H_INCLUDED_) */
