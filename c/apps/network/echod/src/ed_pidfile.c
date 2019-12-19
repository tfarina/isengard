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

#include "ed_pidfile.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ed_log.h"
#include "ed_rcode.h"

int ed_pidfile_write(char const *pidfile_path, pid_t pid) {
  int fd;
  char pidstr[32];
  int pidstr_len;
  ssize_t bytes_written;
  int rc;

  fd = open(pidfile_path, O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) {
    ed_log_error("opening pid file '%s' failed: %s", pidfile_path,
                 strerror(errno));
    return ED_ERROR;
  }

  pidstr_len = snprintf(pidstr, sizeof(pidstr), "%lu\n", (unsigned long) pid);

  bytes_written = write(fd, pidstr, pidstr_len);
  if (bytes_written < 0) {
    ed_log_error("write to pid file '%s' failed: %s", pidfile_path,
                 strerror(errno));
    close(fd);
    return ED_ERROR;
  }

  rc = close(fd);
  if (rc < 0) {
    ed_log_error("close pid file '%s' failed: %s", pidfile_path,
                 strerror(errno));
    return ED_ERROR;
  }

  return ED_OK;
}

int ed_pidfile_remove(char const *pidfile_path) {
  int rc;

  if (pidfile_path == NULL) {
    return ED_ERROR;
  }

  rc = unlink(pidfile_path);
  if (rc < 0) {
    ed_log_error("unlink of pid file '%s' failed, ignored: %s", pidfile_path,
                 strerror(errno));
    return ED_ERROR;
  }

  return ED_OK;
}

