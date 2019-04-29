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
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ed_printf.h"

#define LOG_MAX_LEN 256

static int log_fd = -1;
static ed_log_flag_t log_flags;

static char const * const level_names[] = {
  "ERROR",
  "WARN",
  "NOTICE",
  "INFO",
  "DEBUG",
};

int ed_log_init(char const *filename) {
  if (filename == NULL || !strlen(filename)) {
    log_fd = STDERR_FILENO;
  } else {
    log_fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
      return -1;
    }
  }

  return 0;
}

void ed_log_deinit(void) {
  if (log_fd < 0 || log_fd == STDERR_FILENO) {
    return;
  }

  close(log_fd);

  log_fd = -1;
}

void ed_log_set_flag(ed_log_flag_t flag) {
  log_flags |= flag;
}

void ed_log_write(ed_log_level_t level, char const *file, int line, char const *func, char const *fmt, ...) {
  time_t t;
  struct tm *localtm;
  char timestr[32];
  int len, maxlen;
  va_list args;
  char buf[LOG_MAX_LEN];

  len = 0;
  maxlen = LOG_MAX_LEN;

  if (log_flags & ED_LOG_PRINT_TIME) {
    t = time(NULL);
    localtm = localtime(&t);

    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtm);

    len += ed_scnprintf(buf + len, maxlen - len, "%.*s ", strlen(timestr), timestr);
  }

  if (log_flags & ED_LOG_PRINT_LEVEL) {
    len += ed_scnprintf(buf + len, maxlen - len, "[%s] ", level_names[level]);
  }

  if (log_flags & ED_LOG_PRINT_SRC) {
    len += ed_scnprintf(buf + len, maxlen - len, "%s:%d %s() ", file, line, func);
  }

  va_start(args, fmt);
  len += vsnprintf(buf + len, maxlen - len, fmt, args);
  va_end(args);

  buf[len++] = '\n';

  write(log_fd, buf, len);
}
