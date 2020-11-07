#ifndef _PATHNAMES_H_INCLUDED_
#define _PATHNAMES_H_INCLUDED_ 1

#define DEF_SYSCONFDIR "/etc"

/**
 * Where to keep state files.
 */
#define DEF_STATEDIR "/var/run/echod"

#define DEF_CFGFILE DEF_SYSCONFDIR "/echod.conf"
#define DEF_PIDFILE DEF_STATEDIR "/echod.pid"

#endif  /* !defined(_PATHNAMES_H_INCLUDED_) */
