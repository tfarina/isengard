/*
 * Odyssey.
 *
 * Scalable PostgreSQL connection pooler.
 */

#ifndef ED_LOGGER_H_
#define ED_LOGGER_H_

typedef struct ed_logger_s {
  int fd;
} ed_logger_t;

void ed_logger_init(ed_logger_t *logger);

int ed_logger_open(ed_logger_t *logger, char const *filename);
void ed_logger_close(ed_logger_t *logger);

#endif  /* ED_LOGGER_H_ */
