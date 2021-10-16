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
  char const *str;
  int qtd_on, qtd_pn;
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

  node = mxmlFindElement(xml, xml, "QuantidadeAcaoOrdinariaCapitalIntegralizado", NULL, NULL, MXML_DESCEND);
  if (node == NULL) {
    fputs("Unable to find QuantidadeAcaoOrdinariaCapitalIntegralizado in the XML file.\n", stderr);
    mxmlDelete(xml);
    return 1;
  }

  str = mxmlGetOpaque(node);
  if (!str || *str == '\0') {
    fputs("mxmlGetOpaque failed\n", stderr);
    mxmlDelete(xml);
    return 1;
  }

  qtd_on = atoi(str);

  node = mxmlFindElement(xml, xml, "QuantidadeAcaoPreferencialCapitalIntegralizado", NULL, NULL, MXML_DESCEND);
  if (node == NULL) {
    fputs("Unable to find QuantidadeAcaoPreferencialCapitalIntegralizado in the XML file.\n", stderr);
    mxmlDelete(xml);
    return 1;
  }

  str = mxmlGetOpaque(node);
  if (!str || *str == '\0') {
    fputs("mxmlGetOpaque failed\n", stderr);
    mxmlDelete(xml);
    return 1;
  }

  qtd_pn = atoi(str);

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

  fputs("Composicao do Capital Social:\n", stdout);
  fputs("Numero de acoes (mil):\n", stdout);
  printf("Acoes ordinarias: %d\n", qtd_on);
  printf("Acoes preferenciais: %d\n", qtd_pn);
  printf("Total: %d\n", total);

  mxmlDelete(xml);

  return 0;
}
