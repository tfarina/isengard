#ifndef F_UTILS_H_
#define F_UTILS_H_

/**
 * Gets the current user's home directory.
 */
char *f_get_home_dir(void);

/**
 * Gets the user's configuration directory, where information like
 * user preferences and settings can be stored.
 */
char *f_get_user_config_dir(void);

/**
 * Gets the user name of the current user.
 */
char *f_get_user_name(void);

#endif  /* F_UTILS_H_ */
