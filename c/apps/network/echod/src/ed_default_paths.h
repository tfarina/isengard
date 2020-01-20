#ifndef ED_DEFAULT_PATHS_H_
#define ED_DEFAULT_PATHS_H_

/**
 * The default location containing echod configuration file.
 */
#define DEF_PATH_ECHODCONF SYSCONFDIR"/echod.conf"

/**
 * The file in which to write the process id after the initial fork,
 * when run in daemon (background) mode.
 */
#define DEF_PATH_ECHODPID LOCALSTATEDIR"/run/echod.pid"

#endif  /* ED_DEFAULT_PATHS_H_ */
