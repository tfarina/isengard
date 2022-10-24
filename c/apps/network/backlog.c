#include <stdio.h>
#include <stdlib.h>

static int
getbacklog(void)
{
  FILE *fp;
  char buf[1024];
  int backlog;

  fp = fopen("/proc/sys/net/core/somaxconn","r");
  if (fp == NULL) {
    return -1;
  }

  if (fgets(buf, sizeof(buf), fp) == NULL) {
    fclose(fp);
    return -1;
  }

  backlog = atoi(buf);

  fclose(fp);

  return backlog;
}

int
main(int argc, char **argv)
{
  int backlog;

  backlog = getbacklog();

  printf("%d\n", backlog);

  return 0;
}
