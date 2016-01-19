#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

/* The following two function were copied from scdns-25/scdns.c */

/*
 * Convert a length/string to dotted decimal
 */
static void
todotted(char *a, uint8_t * q)
{
        char *cp;
        if (a != (char *)q)
                strcpy(a, (char *)q);
        cp = a;
        while (*cp) {
                if (*cp < ' ')
                        *cp = '.';
                cp++;
        }
        if (a[0] == '.') {
                cp = a;
                while (*(cp + 1)) {
                        *cp = *(cp + 1);
                        cp++;
                }
                *cp = 0;
        }
}

/*
 * Print a length/string
 */
static void
printundotted(uint8_t *s)
{
        while (*s) {
                if (*s < ' ')
                        printf("(%d)", *s);
                else
                        fputc((int)*s, stdout);
                s++;
        }
        printf("\n");
}

int main(int argc, char **argv) {
  char host[] = "duckduckgo.com";
  char *dnsname= NULL;
  char dotted[256];

  dns_domain_fromdot(host, strlen(host), &dnsname);
  printf("%s\n", dnsname);

  printundotted((uint8_t *)dnsname);

  todotted(dotted, (uint8_t *)dnsname);
  printf("%s\n", dotted);

  return 0;
}
