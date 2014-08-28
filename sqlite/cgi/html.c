#include "html.h"

#include <stdio.h>

void html_content(void) {
  printf("Content-Type: text/html\r\n\r\n");
}

void html_start(void) {
  printf("<!DOCTYPE html>\n");
  printf("<html>\n");
}

void html_end(void) {
  printf("</body>\r\n");
  printf("</html>\r\n");
}
