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

#include "ed_daemon.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "echod.h"

int ed_daemonize(int dump_core)
{
    int rc;
    pid_t pid, sid;
    int fd;

    /* 1st fork detaches child from terminal */
    pid = fork();
    switch (pid) {
    case -1:
      /*log_error("fork() failed: %s", strerror(errno));*/
        return ED_ERROR;

    case 0:
        break;

    default:
        /* parent terminates */
        _exit(0);
    }

    /* 1st child continues and becomes the session and process group leader */
    sid = setsid();
    if (sid < 0) {
        return ED_ERROR;
    }

    if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
      /*log_error("signal(SIGHUP, SIG_IGN) failed: %s", strerror(errno));*/
        return ED_ERROR;
    }

    /* 2nd fork turns child into a non-session leader: cannot acquire terminal */
    pid = fork();
    switch (pid) {
    case -1:
      /*log_error("fork() failed: %s", strerror(errno));*/
        return ED_ERROR;

    case 0:
        break;

    default:
        /* 1st child terminates */
        _exit(0);
    }

    /* change working directory */
    if (dump_core == 0) {
        rc = chdir("/");
        if (rc < 0) {
	  /*log_error("chdir(\"/\") failed: %s", strerror(errno));*/
            return ED_ERROR;
        }
    }

    /* clear file mode creation mask */
    umask(0);

    /* redirect stdin, stdout and stderr to "/dev/null" */

    fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
      /*log_error("open(\"/dev/null\") failed: %s", strerror(errno));*/
        return ED_ERROR;
    }

    rc = dup2(fd, STDIN_FILENO);
    if (rc < 0) {
      /*log_error("dup2(%d, STDIN) failed: %s", fd, strerror(errno));*/
        close(fd);
        return ED_ERROR;
    }

    rc = dup2(fd, STDOUT_FILENO);
    if (rc < 0) {
      /*log_error("dup2(%d, STDOUT) failed: %s", fd, strerror(errno));*/
        close(fd);
        return ED_ERROR;
    }

    rc = dup2(fd, STDERR_FILENO);
    if (rc < 0) {
      /*log_error("dup2(%d, STDERR) failed: %s", fd, strerror(errno));*/
        close(fd);
        return ED_ERROR;
    }

    if (fd > STDERR_FILENO) {
        rc = close(fd);
        if (rc < 0) {
	  /*log_error("close(%d) failed: %s", fd, strerror(errno));*/
            return ED_ERROR;
        }
    }

    return ED_OK;
}
