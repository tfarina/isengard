#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/mxml/mxml.h"

int
main(int argc, char **argv)
{
  char *xmlfile;
  FILE *fp;
  mxml_node_t *tree;
  mxml_node_t *node;
  mxml_node_t *account_plan;
  mxml_node_t *account_version;
  mxml_node_t *fin_code_node;
  mxml_node_t *fi_type;
  mxml_node_t *account_num;
  mxml_node_t *account_description;
  mxml_node_t *account_value;
  int fin_code;
  int fitype;

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
        fin_code_node = mxmlFindElement(account_version, tree, "CodigoTipoDemonstracaoFinanceira", NULL, NULL, MXML_DESCEND);
        if (fin_code_node == NULL) {
          fputs("CodigoTipoDemonstracaoFinanceira not found!\n", stderr);
          break;
        } else {
          /* 2 = demonstracao financeira consolidada */
          fin_code = atoi(mxmlGetOpaque(fin_code_node));
          printf("CodigoTipoDemonstracaoFinanceira: %d\n", fin_code);
        }

        fi_type = mxmlFindElement(account_version, tree, "CodigoTipoInformacaoFinanceira", NULL, NULL, MXML_DESCEND);
        if (fi_type == NULL) {
          fputs("CodigoTipoInformacaoFinanceira not found!\n", stderr);
          break;
        } else {
          fitype = atoi(mxmlGetOpaque(fi_type));
          printf("CodigoTipoInformacaoFinanceira: %d\n", fitype);
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

    account_description = mxmlFindElement(node, tree, "DescricaoConta1", NULL, NULL, MXML_DESCEND);
    if (account_description == NULL) {
      fputs("DescricaoConta1 not found!\n", stderr);
      break;
    } else {
      printf("DescricaoConta1: %s\n", mxmlGetOpaque(account_description));
    }

    account_value = mxmlFindElement(node, tree, "ValorConta1", NULL, NULL, MXML_DESCEND);
    if (account_value == NULL) {
      fputs("ValorConta1 not found!\n", stderr);
      break;
    } else {
      printf("ValorConta1: %s\n", mxmlGetOpaque(account_value));
    }

    node = mxmlFindElement(node, tree, "InfoFinaDFin", NULL, NULL, MXML_DESCEND);
  }

  mxmlDelete(tree);

  return 0;
}
