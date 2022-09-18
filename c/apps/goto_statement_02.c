#include <stdio.h>

int
main(void)
{
  int socket_fd = 0;
  int bind_error = 1;

  if (socket_fd < 0) {
    goto cleanup_generic;
  }

  if (bind_error) {
    goto cleanup_socket;
  }

cleanup_socket:
  printf("Socket closed!\n");
cleanup_generic:
  printf("Generic cleanup!\n");
  return 1;
}
