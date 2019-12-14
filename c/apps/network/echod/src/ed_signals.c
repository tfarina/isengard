#include "ed_signals.h"

#include <signal.h>
#include <stdlib.h>

char const *ed_sig2str(int sig) {
  switch (sig) {
  case SIGINT:
    return "SIGINT";
  case SIGTERM:
    return "SIGTERM";
  }
  return NULL;
}
