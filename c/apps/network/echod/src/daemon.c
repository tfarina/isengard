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

#include "daemon.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ulog.h"

int daemonize(void)
{
    int fd, rc;

    /* 1st fork detaches child from terminal */
    switch (fork()) {
    case -1:
        ulog_error("unable to fork: %s", strerror(errno));
        return -1;

    case 0: /* child */
        break;

    default:
        /* parent terminates */
        _exit(0);
    }

    /* 1st child continues and becomes the session and process group leader */
    if (setsid() < 0) {
        ulog_error("unable to set process group ID: %s", strerror(errno));
        return -1;
    }

    /* 2nd fork turns child into a non-session leader: to ensure that daemon
     * never reacquires a control terminal.
     */
    switch (fork()) {
    case -1:
        ulog_error("unable to fork: %s", strerror(errno));
        return -1;

    case 0: /* child */
        break;

    default:
        /* 1st child terminates */
        _exit(0);
    }

    /* redirect stdin, stdout and stderr to "/dev/null" */

    fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        ulog_error("unable to open /dev/null: %s", strerror(errno));
        return -1;
    }

    rc = dup2(fd, STDIN_FILENO);
    if (rc < 0) {
        ulog_error("unable to dup /dev/null onto stdin: %s", strerror(errno));
        close(fd);
        return -1;
    }

    rc = dup2(fd, STDOUT_FILENO);
    if (rc < 0) {
        ulog_error("unable to dup /dev/null onto stdout: %s", strerror(errno));
        close(fd);
        return -1;
    }

    rc = dup2(fd, STDERR_FILENO);
    if (rc < 0) {
        ulog_error("unable to dup /dev/null onto stderr: %s", strerror(errno));
        close(fd);
        return -1;
    }

    if (fd > STDERR_FILENO) {
        rc = close(fd);
        if (rc < 0) {
	    ulog_error("unable to close /dev/null: %s", strerror(errno));
            return -1;
        }
    }

    return 0;
}
