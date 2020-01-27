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

#include "ed_log.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXLINELEN 1024

static ed_log_dst_t log_dst = ED_LOG_DST_STDERR;
static ed_log_level_t log_level = ED_LOG_LEVEL_INFO;
static char const *log_ident;
static int log_fd = -1;

static char *level_to_str(ed_log_level_t level) {
  switch (level) {
  case ED_LOG_LEVEL_FATAL:
    return "fatal: ";

  case ED_LOG_LEVEL_ERROR:
    return "error: ";

  case ED_LOG_LEVEL_WARN:
    return "warn: ";

  case ED_LOG_LEVEL_INFO:
    return "";

  default:
    return "internal error: ";
  }

  return "unknown: ";
}

static void __vlogmsg(ed_log_level_t level, char const *format, va_list args) {
  time_t now;
  struct tm *localtm;
  char timestr[32];
  int len;
  char buf[MAXLINELEN];

  if (level > log_level) {
    return;
  }

  vsnprintf(buf, sizeof(buf), format, args);
  buf[sizeof(buf) - 1] = '\0'; /* Ensure string is null terminated. */

  if (log_dst & ED_LOG_DST_STDERR) {
    fprintf(stderr, "%s: %s%s\n", log_ident, level_to_str(level), buf);
    fflush(stderr);
  }

  if (log_dst & ED_LOG_DST_FILE && log_fd > 0) {
    time(&now);
    localtm = localtime(&now);
    strftime(timestr, sizeof(timestr), "%Y-%m-%dT%H:%M:%S", localtm);

    len = snprintf(buf, sizeof(buf), "[%.*s] %s", strlen(timestr), timestr, level_to_str(level));
    len += vsnprintf(buf + len, sizeof(buf) - len, format, args);
    buf[len++] = '\n';
    write(log_fd, buf, len);
  }
}

void ed_log_open(char const *ident) {
  if (ident) {
    log_ident = ident;
  }
}

int ed_log_file_open(char const *logfile_path) {
  if (logfile_path == NULL || !strlen(logfile_path)) {
    return -1;
  }

  log_dst |= ED_LOG_DST_FILE;

  log_fd = open(logfile_path, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, S_IRUSR | S_IWUSR);
  if (log_fd < 0) {
    return -1;
  }

  return 0;
}

void ed_log_close(void) {
  if (log_dst & ED_LOG_DST_FILE) {
    if (log_fd < 0) {
      return;
    }

    close(log_fd);
    log_fd = -1;
  }
  log_ident = (char const *) 0;
}

void ed_log_set_level(ed_log_level_t level) {
  log_level = level;
}

void ed_log_fatal(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ED_LOG_LEVEL_FATAL, fmt, ap);
  va_end(ap);

  sleep(1);
  _exit(1);
}

void ed_log_error(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ED_LOG_LEVEL_ERROR, fmt, ap);
  va_end(ap);
}

void ed_log_warn(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ED_LOG_LEVEL_WARN, fmt, ap);
  va_end(ap);
}

void ed_log_info(char const *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  __vlogmsg(ED_LOG_LEVEL_INFO, fmt, ap);
  va_end(ap);
}
