/*
 * pathnames.h - define default paths for echod
 */
#ifndef _PATHNAMES_H_INCLUDED_
#define _PATHNAMES_H_INCLUDED_ 1

#define DEF_SYSCONFDIR "/etc"
#define DEF_LOCALSTATEDIR "/var"

#define DEF_CFGFILE DEF_SYSCONFDIR "/echod.conf"

#define DEF_PIDDIR DEF_LOCALSTATEDIR "/run/echod"
#define DEF_PIDFILE DEF_PIDDIR "/echod.pid"

#endif  /* !defined(_PATHNAMES_H_INCLUDED_) */
