#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/mxml/mxml.h"

/*CodigoTipoDemonstracaoFinanceira*/

int
main(int argc, char **argv)
{
  char *xmlfile;
  FILE *fp;
  mxml_node_t *tree;
  mxml_node_t *node;
  mxml_node_t *num_ident;
  mxml_node_t *account_plan;
  mxml_node_t *account_version;
  mxml_node_t *df_type;
  mxml_node_t *account_num;

  xmlfile = "InfoFinaDFin.xml";
  fp = fopen(xmlfile, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open \"%s\": %s\n", xmlfile, strerror(errno));
    return 1;
  }

  tree = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
  fclose(fp);

  if (tree == NULL) {
    fprintf(stderr, "Unable to read the XML file \"%s\".\n", xmlfile);
    return 1;
  }

  /*ArrayOfInfoFinaDFin*/
  /*  InfoFinaDFin*/
  /*    PlanoConta*/
  /*      NumeroConta*/
  /*    DescricaoConta1*/

  node = mxmlFindElement(tree, tree, "InfoFinaDFin", NULL, NULL, MXML_DESCEND);

  while (node) {
    account_plan = mxmlFindElement(node, tree, "PlanoConta", NULL, NULL, MXML_DESCEND);
    if (account_plan == NULL) {
      fputs("PlanoConta not found!\n", stderr);
      break;
    } else {
      account_version = mxmlFindElement(account_plan, tree, "VersaoPlanoConta", NULL, NULL, MXML_DESCEND);
      if (account_version == NULL) {
        fputs("VersaoPlanoConta not found!\n", stderr);
        break;
      } else {
        df_type = mxmlFindElement(account_version, tree, "CodigoTipoDemonstracaoFinanceira", NULL, NULL, MXML_DESCEND);
        if (df_type == NULL) {
          fputs("CodigoTipoDemonstracaoFinanceira not found!\n", stderr);
          break;
        } else {
          printf("CodigoTipoDemonstracaoFinanceira: %s\n", mxmlGetOpaque(df_type));
        }
      }

      account_num = mxmlFindElement(account_plan, tree, "NumeroConta", NULL, NULL, MXML_DESCEND);
      if (account_num == NULL) {
        fputs("NumeroConta not found!\n", stderr);
        break;
      } else {
        printf("NumeroConta: %s\n", mxmlGetOpaque(account_num));
      }
    }

    num_ident = mxmlFindElement(node, tree, "DescricaoConta1", NULL, NULL, MXML_DESCEND);
    if (num_ident == NULL) {
      fputs("DescricaoConta1 not found!\n", stderr);
      break;
    } else {
      printf("DescricaoConta1: %s\n", mxmlGetOpaque(num_ident));
    }

    node = mxmlFindElement(node, tree, "InfoFinaDFin", NULL, NULL, MXML_DESCEND);
  }

  mxmlDelete(tree);

  return 0;
}
