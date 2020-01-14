#ifndef ED_DEFAULT_PATHS_H_
#define ED_DEFAULT_PATHS_H_

/**
 * The default location of the echod configuration file.
 */
#define ED_CONFIG_FILE_PATH SYSCONFDIR"/echod.conf"

/**
 * The file in which to write the process id after the initial fork,
 * when run in daemon (background) mode.
 */
#define ED_PID_FILE_PATH LOCALSTATEDIR"/run/echod.pid"

#endif  /* ED_DEFAULT_PATHS_H_ */
