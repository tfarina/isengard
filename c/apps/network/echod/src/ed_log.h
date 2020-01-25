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

#ifndef ED_LOG_H_
#define ED_LOG_H_

typedef enum ed_log_dst_e {
  ED_LOG_DST_STDERR = 1 << 0,
  ED_LOG_DST_FILE   = 1 << 1
} ed_log_dst_t;

typedef enum ed_log_level_e {
  ED_LOG_LEVEL_FATAL  = 0,  /* software error (unrecoverable) */
  ED_LOG_LEVEL_ERROR  = 1,  /* recoverable error */
  ED_LOG_LEVEL_WARN   = 2,  /* warning message */
  ED_LOG_LEVEL_INFO   = 3,  /* informative message */
  ED_LOG_LEVEL_MAX
} ed_log_level_t;

void ed_log_set_ident(char const *progname);

int ed_log_file_open(char const *logfile_path);
void ed_log_file_close(void);

void ed_log_set_level(ed_log_level_t level);

void ed_log_fatal(char const *fmt, ...);
void ed_log_error(char const *fmt, ...);
void ed_log_warn(char const *fmt, ...);
void ed_log_info(char const *fmt, ...);

#endif  /* ED_LOG_H_ */
