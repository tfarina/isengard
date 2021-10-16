#include <stdio.h>
#include <stdlib.h>

#include "third_party/mxml/mxml.h"

int
main(int argc, char **argv)
{
  char *filename;
  FILE *fp = NULL;
  mxml_node_t *xml = NULL;
  mxml_node_t *node = NULL; /* MXML_ELEMENT */
  mxml_node_t *child = NULL; /* MXML_OPAQUE */
  char const *str;
  int total;

  filename = "ComposicaoCapitalSocialDemonstracaoFinanceiraNegocios.xml";
  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open \"%s\": %s\n", filename, strerror(errno));
    return 1;
  }

  xml = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
  fclose(fp);

  if (xml == NULL) {
    fprintf(stderr, "Unable to read xml file '%s'.\n", filename);
    return 1;
  }

  node = mxmlFindElement(xml, xml, "QuantidadeTotalAcaoCapitalIntegralizado", NULL, NULL, MXML_DESCEND);
  if (node == NULL) {
    fputs("Unable to find QuantidadeTotalAcaoCapitalIntegralizado in the XML file.\n", stderr);
    mxmlDelete(xml);
    return 1;
  }

  str = mxmlGetOpaque(node);
  if (!str || *str == '\0') {
    fputs("mxmlGetOpaque failed\n", stderr);
    mxmlDelete(xml);
    return 1;
  }

  total = atoi(str);

  fputs("Numero Acoes (mil):\n", stdout);
  printf("Total: %d\n", total);

  mxmlDelete(xml);

  return 0;
}
