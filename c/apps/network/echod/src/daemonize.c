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

#include "daemonize.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ulog.h"

/*
  According to Unix-FAQ maintained by Andrew Gierth:

  1. fork() so the parent can exit, this returns control to the command
  line or shell invoking your program. This step is required so that the
  new process is guaranteed not to be a process group leader. The next
  step, setsid(), fails if you're a process group leader.

  2. setsid() to become a process group and session group leader. Since a
  controlling terminal is associated with a session, and this new session
  has not yet acquired a controlling terminal our process now has no
  controlling terminal, which is a Good Thing for daemons.

  3. fork() again so the parent, (the session group leader), can exit. This
  means that we, as a non-session group leader, can never regain a
  controlling terminal.

  4. chdir("/") to ensure that our process doesn't keep any directory in use.
  Failure to do this could make it so that an administrator couldn't unmount
  a file system, because it was our current directory.
  [Equivalently, we could change to any directory containing files important
  to the daemon's operation.]

  5. umask(0) so that we have complete control over the permissions of
  anything we write. We don't know what umask we may have inherited.
  [This step is optional]

  6. close() fds 0, 1, and 2. This releases the standard in, out, and error
  we inherited from our parent process. We have no way of knowing where
  these fds might have been redirected to. Note that many daemons use
  sysconf() to determine the limit _SC_OPEN_MAX. _SC_OPEN_MAX tells you the
  maximun open files/process. Then in a loop, the daemon can close all
  possible file descriptors. You have to decide if you need to do this or not.
  If you think that there might be file-descriptors open you should close
  them, since there's a limit on number of concurrent file descriptors.

  7. Establish new open descriptors for stdin, stdout and stderr. Even if
  you don't plan to use them, it is still a good idea to have them open.
  The precise handling of these is a matter of taste; if you have a logfile,
  for example, you might wish to open it as stdout or stderr, and open
  `/dev/null' as stdin; alternatively, you could open `/dev/console' as
  stderr and/or stdout, and `/dev/null' as stdin, or any other combination
  that makes sense for your particular daemon.
*/

int daemonize(void)
{
    int fd, rc, failed = 0;

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
        failed++;
    }

    rc = dup2(fd, STDOUT_FILENO);
    if (rc < 0) {
        ulog_error("unable to dup /dev/null onto stdout: %s", strerror(errno));
        failed++;
    }

    rc = dup2(fd, STDERR_FILENO);
    if (rc < 0) {
        ulog_error("unable to dup /dev/null onto stderr: %s", strerror(errno));
        failed++;
    }

    if (fd > STDERR_FILENO) {
        (void) close(fd);
    }

    if (failed) {
        return -1;
    }

    return 0;
}
