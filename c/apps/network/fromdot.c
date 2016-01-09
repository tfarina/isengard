#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Based on Daniel Julius Bernstein (DJB's)'s public domain code:
 * djbdns-1.05/dns_dfd.c
 */
int dns_domain_fromdot(const char *buf,
                       size_t len,
                       char **out) {
  char label[63];
  unsigned int labellen = 0;
  char name[255];
  unsigned int namelen = 0;
  char ch;
  char *x;

  for (;;) {
    if (!len)
      break;

    ch = *buf++;
    --len;

    if (ch == '.') {
      if (!labellen)
        return 0;

      if (namelen + labellen + 1 > sizeof(name))
        return 0;
      name[namelen++] = (char)labellen;
      memcpy(name + namelen, label, labellen);
      namelen += labellen;
      labellen = 0;
      continue;
    }

    if (labellen >= sizeof(label))
      return 0;
    label[labellen++] = ch;
  }

  if (labellen) {
    if (namelen + labellen + 1 > sizeof(name))
      return 0;
    name[namelen++] = (char)labellen;
    memcpy(name + namelen, label, labellen);
    namelen += labellen;
    labellen = 0;
  }

  if (namelen + 1 > sizeof(name))
    return 0;
  if (namelen == 0)
    return 0;
  name[namelen++] = 0;

  x = malloc(namelen);
  if (!x)
    return 0;
  memcpy(x, name, namelen);

  if (*out)
    free(*out);

  *out = x;
  return 1;
}

int main(int argc, char **argv) {
  char host[] = "duckduckgo.com";
  char *dnsname = NULL;

  dns_domain_fromdot(host, strlen(host), &dnsname);

  printf("%s\n", dnsname);

  return 0;
}
