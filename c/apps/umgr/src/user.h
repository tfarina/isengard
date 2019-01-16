#ifndef USER_H_
#define USER_H_

/**
 * Returns 0 if the user does not exists, otherwise returns 1.
 */
int user_exists(const char* email);

int user_add(const char* fname,
             const char* lname,
             const char* email);

int user_change(const char *fname, const char *email);

int user_delete(const char *email);

/**
 * Lists records from the user table.
 */
int user_list(void);

#endif  /* USER_H_ */
