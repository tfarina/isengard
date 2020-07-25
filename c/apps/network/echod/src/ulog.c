/*
 * Odyssey.
 *
 * Scalable PostgreSQL connection pooler.
 */

/*
 * twemcache - Twitter memcached.
 * Copyright (c) 2012, Twitter, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the Twitter nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ulog.h"

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAXLINELEN 1024
#define RFC5424_TIMESTAMP "%Y-%m-%dT%H:%M:%S"

#define ULOG_DST_TERM    0
#define ULOG_DST_FILE    1

static char log_ident[32];
static ulog_level_t log_level = ULOG_INFO;
static int log_dst = ULOG_DST_TERM;
static int log_fd = -1;

typedef struct {
  int value;
  char const *name;
} ulog_code_t;

static ulog_code_t const levelnames[] = {
  { ULOG_FATAL, "fatal"   },
  { ULOG_ERROR, "error"   },
  { ULOG_WARN,  "warning" },
  { ULOG_INFO,  ""        },
  { -1,         0         }
};

static char const *level_to_str(ulog_level_t level) {
  switch (level) {
  case ULOG_FATAL: return "fatal: ";
  case ULOG_ERROR: return "error: ";
  case ULOG_WARN:  return "warning: ";
  case ULOG_INFO:  return "";
  default:         return "internal error: ";
  }
}

/**
 * Generates a log message using 'fmt' string, followed by the variable
 * arguments in 'ap'.
 */
static void __vlogmsg(ulog_level_t level, char const *fmt, va_list ap) {
  time_t now;
  struct tm *localtm;
  char timebuf[32];
  char buf[MAXLINELEN];
  int len;
  int fd;

  /*
   * Check if user is interested in this log message.
   */
  if (level > log_level) {
    return;
  }

  if (log_dst == ULOG_DST_TERM) {
    fd = (level == ULOG_INFO) ? STDOUT_FILENO : STDERR_FILENO;

    len = snprintf(buf, sizeof(buf), "\r%s: %s", log_ident, level_to_str(level));
    len += vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
    buf[len++] = '\n';
    buf[sizeof(buf) - 1] = '\0'; /* Ensure the buffer is NUL-terminated. */
    write(fd, buf, len);
  }

  if (log_dst == ULOG_DST_FILE && log_fd > 0) {
    time(&now);
    localtm = localtime(&now);
    strftime(timebuf, sizeof(timebuf), RFC5424_TIMESTAMP, localtm);

    len = snprintf(buf, sizeof(buf), "[%.*s] %s", strlen(timebuf), timebuf, level_to_str(level));
    len += vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
    buf[len++] = '\n';
    buf[sizeof(buf) - 1] = '\0'; /* Ensure the buffer is NUL-terminated. */
    write(log_fd, buf, len);
  }
}

void ulog_open(char const *ident) {
  if (ident) {
    strncpy(log_ident, ident, sizeof(log_ident));
    log_ident[sizeof(log_ident) - 1] = 0;
  } else {
    *log_ident = 0;
  }
}

void ulog_set_file(char const *log_file) {
  int fd;

  if (log_file == NULL || *log_file == '\0') {
    return;
  }

  fd = open(log_file, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    return;
  }

  log_fd = fd;
  log_dst = ULOG_DST_FILE;
}

/**
 * Closes the file descriptor being used to write to the file.
 */
void ulog_close(void) {
  if (log_dst == ULOG_DST_FILE) {
    if (log_fd < 0) {
      return;
    }

    close(log_fd);
    log_fd = -1;
  }
}

void ulog_fatal(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_FATAL, fmt, ap);
  va_end(ap);

  sleep(1);
  _exit(1);
}

void ulog_error(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_ERROR, fmt, ap);
  va_end(ap);
}

void ulog_warn(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_WARN, fmt, ap);
  va_end(ap);
}

void ulog_info(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_INFO, fmt, ap);
  va_end(ap);
}
