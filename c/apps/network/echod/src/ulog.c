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
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXLINELEN 1024
#define RFC5424_TIMESTAMP "%Y-%m-%dT%H:%M:%S"

#define ULOG_DST_CONSOLE (1 << 0)
#define ULOG_DST_FILE    (1 << 1)

static char const *log_ident;
static ulog_level_t log_level = ULOG_LEVEL_INFO;
static int log_dst = ULOG_DST_CONSOLE;
static int log_fd = -1;

static char *level_to_str(ulog_level_t level) {
  switch (level) {
  case ULOG_LEVEL_FATAL:
    return "fatal: ";

  case ULOG_LEVEL_ERROR:
    return "error: ";

  case ULOG_LEVEL_WARN:
    return "warning: ";

  case ULOG_LEVEL_INFO:
    return "";

  default:
    return "internal error: ";
  }

  return "unknown: ";
}

static void __vlogmsg(ulog_level_t level, char const *fmt, va_list ap) {
  time_t now;
  struct tm *localtm;
  char timebuf[32];
  char buf[MAXLINELEN];
  int len;
  FILE *term_file;

  if (level > log_level) {
    return;
  }

  vsnprintf(buf, sizeof(buf), fmt, ap);
  buf[sizeof(buf) - 1] = '\0'; /* Ensure the buffer is NUL-terminated. */

  if (log_dst & ULOG_DST_CONSOLE) {
    if (level == ULOG_LEVEL_INFO) {
      term_file = stdout;
    } else {
      term_file = stderr;
    }
    fprintf(term_file, "\r%s: %s%s\n", log_ident, level_to_str(level), buf);
    fflush(term_file);
  }

  if (log_dst & ULOG_DST_FILE && log_fd > 0) {
    time(&now);
    localtm = localtime(&now);
    strftime(timebuf, sizeof(timebuf), RFC5424_TIMESTAMP, localtm);

    len = snprintf(buf, sizeof(buf), "[%.*s] %s", strlen(timebuf), timebuf, level_to_str(level));
    len += vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
    buf[len++] = '\n';
    write(log_fd, buf, len);
  }
}

void ulog_open(char const *ident, char const *logfile_path) {
  if (ident) {
    log_ident = ident;
  }

  if (logfile_path == NULL || !strlen(logfile_path)) {
    return;
  }

  log_dst |= ULOG_DST_FILE;

  log_fd = open(logfile_path, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, S_IRUSR | S_IWUSR);
}

void ulog_close(void) {
  if (log_dst & ULOG_DST_FILE) {
    if (log_fd < 0) {
      return;
    }

    close(log_fd);
    log_fd = -1;
  }
  log_ident = (char const *) 0;
}

void ulog_fatal(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_LEVEL_FATAL, fmt, ap);
  va_end(ap);

  sleep(1);
  _exit(1);
}

void ulog_error(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_LEVEL_ERROR, fmt, ap);
  va_end(ap);
}

void ulog_warn(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_LEVEL_WARN, fmt, ap);
  va_end(ap);
}

void ulog_info(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ULOG_LEVEL_INFO, fmt, ap);
  va_end(ap);
}
