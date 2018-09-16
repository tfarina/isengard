#include "ed_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/syslog.h>

static int log_on_stderr = 1;

void log_init(const char *progname, int on_stderr) {
  log_on_stderr = on_stderr;

  if (!log_on_stderr)
    openlog(progname, LOG_PID | LOG_NDELAY, LOG_DAEMON);
}

void vlog(int pri, const char *fmt, va_list ap) {
  char *nfmt;
  if (log_on_stderr) {
    /* Write to stderr */
    if (asprintf(&nfmt, "%s\n", fmt) == -1) {
      vfprintf(stderr, fmt, ap);
      fprintf(stderr, "\n");
    } else {
      vfprintf(stderr, nfmt, ap);
      free(nfmt);
    }
    fflush(stderr);
  } else {
    vsyslog(pri, fmt, ap);
  }
}

void log_info(const char *emsg, ...) {
  va_list ap;

  va_start(ap, emsg);
  vlog(LOG_INFO, emsg, ap);
  va_end(ap);
}

void log_error(const char *emsg, ...) {
  va_list ap;

  va_start(ap, emsg);
  vlog(LOG_ERR, emsg, ap);
  va_end(ap);
}
