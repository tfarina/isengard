#include <stdio.h>

#include "third_party/mxml/mxml.h"

int
main(int argc, char **argv)
{
  char *filename;
  FILE *fp = NULL;
  mxml_node_t *xml = NULL;
  mxml_node_t *node = NULL;
  mxml_node_t *child = NULL;

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
    fputs("Unable to find this element in XML tree.\n", stderr);
    mxmlDelete(xml);
    return 1;
  }

  mxml_type_t type;

  type = mxmlGetType(node);

  switch(type) {
  case  MXML_ELEMENT: printf("ELEMENT\n");break;
  case   MXML_INTEGER: printf("INTEGER\n"); break;
  case MXML_OPAQUE: printf("OPAQUE\n"); break;
  case MXML_REAL: printf("REAL\n"); break;
  case MXML_TEXT: printf("TEXT\n");break;
  case   MXML_CUSTOM	: printf("CUSTOM\n"); break;
  }

  child = mxmlGetFirstChild(node);

  type = mxmlGetType(child);

  switch(type) {
  case  MXML_ELEMENT: printf("ELEMENT\n");break;
  case   MXML_INTEGER: printf("INTEGER\n"); break;
  case MXML_OPAQUE: printf("OPAQUE\n"); break;
  case MXML_REAL: printf("REAL\n"); break;
  case MXML_TEXT: printf("TEXT\n");break;
  case   MXML_CUSTOM	: printf("CUSTOM\n"); break;
  }

  puts("Numero Acoes (mil):");
  printf("Total: %s\n", mxmlGetOpaque(child));

  mxmlDelete(xml);

  return 0;
}
