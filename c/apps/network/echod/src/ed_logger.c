/*
 * Odyssey.
 *
 * Scalable PostgreSQL connection pooler.
 */

#include "ed_logger.h"

#include <fcntl.h>
#include <unistd.h>

void ed_logger_init(ed_logger_t *logger) {
  logger->fd = -1;
}

int ed_logger_open(ed_logger_t *logger, char const *filename) {
  logger->fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644);
  if (logger->fd == -1) {
    return -1;
  }

  return 0;
}

void ed_logger_close(ed_logger_t *logger) {
  if (logger->fd != -1) {
    close(logger->fd);
  }

  logger->fd = -1;
}

