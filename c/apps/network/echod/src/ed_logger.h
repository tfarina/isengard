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

#ifndef ED_LOGGER_H_
#define ED_LOGGER_H_

#include <stdarg.h>

typedef enum ed_logger_level_e {
  ED_LOGGER_LEVEL_DEBUG = 0,
  ED_LOGGER_LEVEL_NOTICE,
  ED_LOGGER_LEVEL_INFO,
  ED_LOGGER_LEVEL_WARN,
  ED_LOGGER_LEVEL_ERROR,
  ED_LOGGER_LEVEL_MAX
} ed_logger_level_t;

int ed_logger_init(char const *filename);
void ed_logger_deinit(void);

void ed_logger_write(ed_logger_level_t level,
		     char const *file,
		     int line,
		     char const *func,
		     char const *format, ...);

#define ed_logger_log_debug(...) ed_logger_write(ED_LOGGER_LEVEL_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ed_logger_log_notice(...) ed_logger_write(ED_LOGGER_LEVEL_NOTICE, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ed_logger_log_info(...) ed_logger_write(ED_LOGGER_LEVEL_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ed_logger_log_warn(...) ed_logger_write(ED_LOGGER_LEVEL_WARN, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ed_logger_log_error(...) ed_logger_write(ED_LOGGER_LEVEL_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)

#endif  /* ED_LOGGER_H_ */
