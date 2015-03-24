#include "str_cli.h"

#include <string.h>

#include "readline.h"
#include "wrapstdio.h"
#include "writen.h"

/* Max text line length. */
#define MAXLINE 4096

void str_cli(FILE *fp, int sockfd) {
  char sendline[MAXLINE];
  char recvline[MAXLINE];

  while (Fgets(sendline, MAXLINE, fp) != NULL) {
    Writen(sockfd, sendline, strlen(sendline));
    if (Readline(sockfd, recvline, MAXLINE) == 0) {
      //err_quit("str_cli: server terminated prematurely");
    }
    Fputs(recvline, stdout);
  }
}
