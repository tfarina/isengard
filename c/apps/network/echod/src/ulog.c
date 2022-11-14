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

/*
 * File permissions for Log file, -rw-------, 0600.
 *
 * Owner or user permissions: After the directory (d) slot, the first set of
 * three characters indicate permission settings for the owner (also known as
 * the user).
 *
 * In this case, the owner permissions are rw-, indicating that the
 * owner (user) can read and write to the file but can't execute it
 * as a program.
 *
 * Absolute form:
 * The three numbers are specified in the order: user (or owner), group, and
 * other. Each number is the sum of values that specify read, write, and
 * execute access:
 * Permission | Number
 * Read(r)    | 4
 * Write(w)   | 2
 * Execute(x) | 1
 *
 * 400 + 200 = 600
 */
#define FILE_PERM (S_IRUSR | S_IWUSR)

/*
 * Timestamp according to https://tools.ietf.org/html/rfc5424#section-6.2.3
 */
#define DATETIMEFORMAT "%Y-%m-%dT%H:%M:%S"

#define ULOG_DST_TERM    0
#define ULOG_DST_FILE    1

static char log_ident[32];
static ulog_level_t current_loglevel = ULOG_INFO;
static int log_dst = ULOG_DST_TERM;
static int log_fd = -1;

/**
 * Converts a #ulog_level_t to a string.
 *
 * @return identifier string
 */
static char const *__ulog_level_to_str(ulog_level_t level) {
  switch (level) {
  case ULOG_FATAL: return "fatal: ";
  case ULOG_ERROR: return "error: ";
  case ULOG_WARN:  return "warning: ";
  case ULOG_NOTICE:return "";
  case ULOG_INFO:  return "";
  default:         return "unknown";
  }
}

/**
 * Generates a log message using 'fmt' string, followed by the variable
 * arguments in 'ap'.
 */
static void __vlogmsg(ulog_level_t level, char const *fmt, va_list ap) {
  int fd;
  int len;
  char buf[MAXLINELEN];
  time_t now;
  struct tm *localtm;
  char timebuf[32];

  /*
   * Check if user is interested in logging this message.
   *
   * Example: If current level is at WARN (2) and the code came here through
   * calling ulog_info function, then level will be INFO (4), which means in
   * this case with the current settings we are not interested in seeing
   * info messages. To see them the user would have to increase the verbosity
   * level.
   */
  if (level > current_loglevel) {
    return;
  }

  if (log_dst == ULOG_DST_TERM) {
    fd = (level == ULOG_INFO) ? STDOUT_FILENO : STDERR_FILENO;

    len = snprintf(buf, sizeof(buf), "\r%s", __ulog_level_to_str(level));
    len += vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
    buf[len++] = '\n';
    buf[sizeof(buf) - 1] = '\0'; /* Ensure the buffer is NUL-terminated. */
    write(fd, buf, len);
  }

  if (log_dst == ULOG_DST_FILE && log_fd > 0) {
    time(&now);
    localtm = localtime(&now);
    strftime(timebuf, sizeof(timebuf), DATETIMEFORMAT, localtm);

    len = snprintf(buf, sizeof(buf), "[%s] %s", timebuf, __ulog_level_to_str(level));
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

int ulog_set_file(char const *log_file) {
  int fd;

  if (log_file == NULL || *log_file == '\0') {
    return -1;
  }

  fd = open(log_file, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, FILE_PERM);
  if (fd < 0) {
    return -1;
  }

  log_fd = fd;
  log_dst = ULOG_DST_FILE;

  return 0;
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

void ulog_notice(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_NOTICE, fmt, ap);
  va_end(ap);
}

void ulog_info(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_INFO, fmt, ap);
  va_end(ap);
}
