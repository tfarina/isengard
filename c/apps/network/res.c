#include <arpa/inet.h>
#include <resolv.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  struct __res_state res;
  int i;

  memset(&res, 0, sizeof(res));

  if (res_ninit(&res) == -1) {
    fprintf(stderr, "res_ninit failed\n");
    exit(EXIT_FAILURE);
  }

  /* Initially, glibc stores IPv6 in |_ext.nsaddrs| and IPv4 in |nsaddr_list|. */
  for (i = 0; i < res.nscount; ++i) {
    const struct sockaddr* addr = NULL;
    size_t addr_len = 0;
    char str[INET6_ADDRSTRLEN];
    if (res.nsaddr_list[i].sin_family) {
      addr = (const struct sockaddr*)&res.nsaddr_list[i];
      addr_len = sizeof(res.nsaddr_list[i]);
      inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), str, INET_ADDRSTRLEN);
      printf("IPv4 %d: %s\n", i, str);
    } else if (res._u._ext.nsaddrs[i] != NULL) {
      addr = (const struct sockaddr*)res._u._ext.nsaddrs[i];
      addr_len = sizeof(*res._u._ext.nsaddrs[i]);
      inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)addr)->sin6_addr), str, INET6_ADDRSTRLEN);
      printf("IPv6 %d: %s\n", i, str);
    }

    /* After you get the nameservers from resolv.conf, what do you do?
     * Send the dns query to one of them? All?
     */
  }

  res_nclose(&res);

  return 0;
}
