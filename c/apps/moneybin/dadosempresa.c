#include <stdio.h>

#include "third_party/mxml/mxml.h"

int
main(int argc, char **argv)
{
  char *filename;
  FILE *fp = NULL;
  mxml_node_t *xml = NULL;
  mxml_node_t *node = NULL; /* MXML_ELEMENT */
  mxml_node_t *child = NULL; /* MXML_OPAQUE */
  mxml_type_t type;

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

  child = mxmlGetFirstChild(node);

  if (child != NULL) {
    puts("Numero Acoes (mil):");
    printf("Total: %s\n", mxmlGetOpaque(child));
  }

  mxmlDelete(xml);

  return 0;
}
