#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/mxml/mxml.h"

typedef struct account_s {
  char *number;
  char *description;
  char *value;
} account_t;

int
main(int argc, char **argv)
{
  char *xmlfile;
  FILE *fp;
  mxml_node_t *tree;
  mxml_node_t *node;
  mxml_node_t *acc_plan_node;
  mxml_node_t *acc_version_node;
  mxml_node_t *fin_code_node;
  mxml_node_t *inf_code_node;
  mxml_node_t *acc_num_node;
  mxml_node_t *acc_desc_node;
  mxml_node_t *acc_value_node;
  int fin_code;
  int inf_code;
  account_t *account;
  account_t **accounts = NULL; /* array of pointers to accounts */
  unsigned nb_accounts = 0; /* number of accounts */
  char const *str;

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

  /* ArrayOfInfoFinaDFin
   *   InfoFinaDFin
   *     PlanoConta
   *       NumeroConta
   *     DescricaoConta1
   *     ValorConta1
   *   InfoFinaDFin
   *     PlanoConta
   *       NumeroConta
   *     DescricaoConta1
   *     ValorConta1
   */

  node = mxmlFindElement(tree, tree, "InfoFinaDFin", NULL, NULL, MXML_DESCEND);

  while (node) {
    /* This is a small trick.
     * In the first pass 'accounts' will be NULL, then this realloc call
     * will be equivalent to malloc(sizeof(account_t) * 1). */
    accounts = realloc(accounts, sizeof(account_t) * nb_accounts + 1);
    if (accounts == NULL) {
      fputs("Out of memory\n", stderr);
      break;
    }

    account = malloc(sizeof(account_t));
    if (account == NULL) {
      fputs("out of memory!\n", stderr);
      break;
    }

    accounts[nb_accounts++] = account;

    acc_plan_node = mxmlFindElement(node, tree, "PlanoConta", NULL, NULL, MXML_DESCEND);
    if (acc_plan_node == NULL) {
      fputs("PlanoConta not found!\n", stderr);
      break;
    } else {
      acc_version_node = mxmlFindElement(acc_plan_node, tree, "VersaoPlanoConta", NULL, NULL, MXML_DESCEND);
      if (acc_version_node == NULL) {
        fputs("VersaoPlanoConta not found!\n", stderr);
        break;
      } else {
        fin_code_node = mxmlFindElement(acc_version_node, tree, "CodigoTipoDemonstracaoFinanceira", NULL, NULL, MXML_DESCEND);
        if (fin_code_node == NULL) {
          fputs("CodigoTipoDemonstracaoFinanceira not found!\n", stderr);
          break;
        } else {
          str = mxmlGetOpaque(fin_code_node);
          if (!str || *str == '\0') {
            fputs("mxmlGetOpaque failed\n", stderr);
            break;
          }

          /* 2 = demonstracao financeira consolidada */
          fin_code = atoi(str);
          printf("CodigoTipoDemonstracaoFinanceira: %d\n", fin_code);
        }

        inf_code_node = mxmlFindElement(acc_version_node, tree, "CodigoTipoInformacaoFinanceira", NULL, NULL, MXML_DESCEND);
        if (inf_code_node == NULL) {
          fputs("CodigoTipoInformacaoFinanceira not found!\n", stderr);
          break;
        } else {
          str = mxmlGetOpaque(inf_code_node);
          if (!str || *str == '\0') {
            fputs("mxmlGetOpaque failed\n", stderr);
            break;
          }

          inf_code = atoi(str);
          printf("CodigoTipoInformacaoFinanceira: %d\n", inf_code);
        }
      }

      acc_num_node = mxmlFindElement(acc_plan_node, tree, "NumeroConta", NULL, NULL, MXML_DESCEND);
      if (acc_num_node == NULL) {
        fputs("NumeroConta not found!\n", stderr);
        break;
      } else {
        str = mxmlGetOpaque(acc_num_node);
        if (!str || *str == '\0') {
          fputs("mxmlGetOpaque failed\n", stderr);
          break;
        }

        printf("NumeroConta: %s\n", str);
      }
    }

    acc_desc_node = mxmlFindElement(node, tree, "DescricaoConta1", NULL, NULL, MXML_DESCEND);
    if (acc_desc_node == NULL) {
      fputs("DescricaoConta1 not found!\n", stderr);
      break;
    } else {
      str = mxmlGetOpaque(acc_desc_node);
      if (!str || *str == '\0') {
        fputs("mxmlGetOpaque failed\n", stderr);
        break;
      }

      printf("DescricaoConta1: %s\n", str);
    }

    acc_value_node = mxmlFindElement(node, tree, "ValorConta1", NULL, NULL, MXML_DESCEND);
    if (acc_value_node == NULL) {
      fputs("ValorConta1 not found!\n", stderr);
      break;
    } else {
      str = mxmlGetOpaque(acc_value_node);
      if (!str || *str == '\0') {
        fputs("mxmlGetOpaque failed\n", stderr);
        break;
      }

      printf("ValorConta1: %s\n", str);
    }

    node = mxmlFindElement(node, tree, "InfoFinaDFin", NULL, NULL, MXML_DESCEND);
  }

  mxmlDelete(tree);

  return 0;
}
