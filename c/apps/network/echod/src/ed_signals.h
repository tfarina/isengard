#ifndef ED_SIGNALS_H_
#define ED_SIGNALS_H_

/**
 * Returns the name of an UNIX signal. Similar to Solaris sig2str().
 */
char const *ed_sig2str(int sig);

#endif  /* ED_SIGNALS_H_ */
