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

#include "pidfile.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ulog.h"

#define PIDSTRLEN 12

/*
 * File permissions for PID file, rw-r--r--, 0644.
 */
#define FILE_PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int pidfile_write(char const *pidfile_path, pid_t pid) {
  int fd;
  char pidstr[PIDSTRLEN + 1]; /* +1 to include the terminating null byte ('\0'). */
  int len;
  ssize_t bytes_written;
  int rc;

  fd = open(pidfile_path, O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC, FILE_PERM);
  if (fd < 0) {
    ulog_error("unable to open pidfile '%s': %s", pidfile_path,
               strerror(errno));
    return -1;
  }

  /*
   * The |pidstr| array goes from 0 to 12.
   *
   * PIDSTRLEN - 1 = 11 characters or bytes.
   *
   * That means:
   * pidstr[11] = '\n'
   * pidstr[12] = '0'
   */
  len = snprintf(pidstr, sizeof(pidstr), "%*ld\n", PIDSTRLEN - 1, (unsigned long) pid);
  if (len < 0) {
    ulog_error("unable to convert PID to string: %s", strerror(errno));
    close(fd);
    return -1;
  }

  /*
   * Asking write(3) to write 12 bytes, upto '\n', and thus
   * excluding the terminating null byte ('\0').
   */
  bytes_written = write(fd, pidstr, PIDSTRLEN);
  if (bytes_written < 0 || bytes_written != PIDSTRLEN) {
    ulog_error("unable to write to pidfile '%s': %s", pidfile_path,
               strerror(errno));
    close(fd);
    return -1;
  }

  rc = close(fd);
  if (rc < 0) {
    ulog_error("unable to close pidfile '%s': %s", pidfile_path,
               strerror(errno));
    return -1;
  }

  return 0;
}

int pidfile_remove(char const *pidfile_path) {
  int rc;

  if (pidfile_path == NULL) {
    return -1;
  }

  rc = unlink(pidfile_path);
  if (rc < 0) {
    ulog_error("unable to remove pidfile '%s': %s", pidfile_path, strerror(errno));
    return -1;
  }

  return 0;
}
