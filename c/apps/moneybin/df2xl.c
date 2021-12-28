#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fstrdup.h"
#include "third_party/mxml/mxml.h"
#include "third_party/libxlsxwriter/include/xlsxwriter.h"

typedef enum balance_type_e {
  DFP_BALANCE_TYPE_INVALID = 0,
  DFP_BALANCE_TYPE_IF = 1,
  DFP_BALANCE_TYPE_BPA = 2,  /** Balanços Patrimoniais Activos */
  DFP_BALANCE_TYPE_BPP = 3,  /** Balanços Patrimoniais Passivos */
  DFP_BALANCE_TYPE_DRE = 4,  /** Demonstração do Resultado do Exercicio */
  DFP_BALANCE_TYPE_DRA = 5,  /** Demonstraçao do Resultado Abrangente */
  DFP_BALANCE_TYPE_DFC_MD = 6,  /** Demonstração do Fluxo de Caixa - Método Direto */
  DFP_BALANCE_TYPE_DFC_MI = 7,  /** Demonstração do Fluxo de Caixa - Método Indireto */
  DFP_BALANCE_TYPE_DMPL = 8,  /** Demonstração das Mutaçoes do Patrimônio Líquido */
  DFP_BALANCE_TYPE_DVA = 9  /** Demonstração do Valor Adicionado */
} balance_type_t;

typedef enum financial_info_type_e {
  DFP_FINANCIAL_INFO_TYPE_INDIVIDUAL = 1,
  DFP_FINANCIAL_INFO_TYPE_CONSOLIDATED = 2
} financial_info_type_t;

typedef struct account_s {
  char const *number;
  char const *description;
  long long value;
  balance_type_t balance_type;
  financial_info_type_t financial_info_type;
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
  int balance_type;
  int financial_info_type;
  account_t *account;
  account_t **accounts = NULL; /* array of pointers to accounts */
  unsigned nb_accounts = 0; /* number of accounts */
  int i;
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

  for (node = mxmlFindElement(tree, tree, "InfoFinaDFin", NULL, NULL, MXML_DESCEND);
       node;
       node = mxmlFindElement(node, tree, "InfoFinaDFin", NULL, NULL, MXML_DESCEND)) {
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

    account->number = NULL;
    account->description = NULL;
    account->value = 0;

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

          /* enum balance_type_e */
          balance_type = atoi(str);
          account->balance_type = balance_type;
          printf("CodigoTipoDemonstracaoFinanceira: %d\n", balance_type);
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

          /* enum financial_info_type_e */
          financial_info_type = atoi(str);
          account->financial_info_type = financial_info_type;
          printf("CodigoTipoInformacaoFinanceira: %d\n", financial_info_type);
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

        account->number = f_strdup(str);

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

      account->description = f_strdup(str);

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

      account->value = atoll(str);

      printf("ValorConta1: %s\n", str);
    }
  }

  mxmlDelete(tree);

  for (i = 0; i < nb_accounts; i++) {
    printf("%s %s %lld\n", accounts[i]->number, accounts[i]->description, accounts[i]->value);
  }

  lxw_workbook  *workbook;
  lxw_worksheet *worksheet_bpa;
  lxw_worksheet *worksheet_bpp;
  lxw_worksheet *worksheet_dre;
  lxw_worksheet *worksheet_dra;
  lxw_worksheet *worksheet_dfc_md;
  lxw_worksheet *worksheet_dfc_mi;
  lxw_worksheet *worksheet_dmpl;
  lxw_worksheet *worksheet_dva;

  workbook = workbook_new("ind_df_export.xlsx");

  worksheet_bpa = workbook_add_worksheet(workbook, "BPA");

  worksheet_write_string(worksheet_bpa, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_bpa, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_bpa, 0, 2, "VL_CONTA", NULL);

  worksheet_bpp = workbook_add_worksheet(workbook, "BPP");

  worksheet_write_string(worksheet_bpp, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_bpp, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_bpp, 0, 2, "VL_CONTA", NULL);

  worksheet_dre = workbook_add_worksheet(workbook, "DRE");

  worksheet_write_string(worksheet_dre, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_dre, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_dre, 0, 2, "VL_CONTA", NULL);

  worksheet_dra = workbook_add_worksheet(workbook, "DRA");

  worksheet_write_string(worksheet_dra, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_dra, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_dra, 0, 2, "VL_CONTA", NULL);

  worksheet_dfc_md = workbook_add_worksheet(workbook, "DFC_MD");

  worksheet_write_string(worksheet_dfc_md, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_dfc_md, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_dfc_md, 0, 2, "VL_CONTA", NULL);

  worksheet_dfc_mi = workbook_add_worksheet(workbook, "DFC_MI");

  worksheet_write_string(worksheet_dfc_mi, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_dfc_mi, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_dfc_mi, 0, 2, "VL_CONTA", NULL);

  worksheet_dmpl = workbook_add_worksheet(workbook, "DMPL");

  worksheet_write_string(worksheet_dmpl, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_dmpl, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_dmpl, 0, 2, "VL_CONTA", NULL);

  worksheet_dva = workbook_add_worksheet(workbook, "DVA");

  worksheet_write_string(worksheet_dva, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_dva, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_dva, 0, 2, "VL_CONTA", NULL);

  int row_index_bpa = 1;
  int row_index_bpp = 1;
  int row_index_dre = 1;
  int row_index_dra = 1;
  int row_index_dfc_md = 1;
  int row_index_dfc_mi = 1;
  int row_index_dmpl = 1;
  int row_index_dva = 1;
  for (i = 0; i < nb_accounts; i++) {
    if (accounts[i]->financial_info_type == DFP_FINANCIAL_INFO_TYPE_INDIVIDUAL) {
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_BPA) {
        worksheet_write_string(worksheet_bpa, row_index_bpa, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_bpa, row_index_bpa, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_bpa, row_index_bpa, 2, accounts[i]->value, NULL);
        row_index_bpa++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_BPP) {
        worksheet_write_string(worksheet_bpp, row_index_bpp, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_bpp, row_index_bpp, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_bpp, row_index_bpp, 2, accounts[i]->value, NULL);
        row_index_bpp++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_DRE) {
        worksheet_write_string(worksheet_dre, row_index_dre, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_dre, row_index_dre, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_dre, row_index_dre, 2, accounts[i]->value, NULL);
        row_index_dre++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_DRA) {
        worksheet_write_string(worksheet_dra, row_index_dra, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_dra, row_index_dra, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_dra, row_index_dra, 2, accounts[i]->value, NULL);
        row_index_dra++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_DFC_MD) {
        worksheet_write_string(worksheet_dfc_md, row_index_dfc_md, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_dfc_md, row_index_dfc_md, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_dfc_md, row_index_dfc_md, 2, accounts[i]->value, NULL);
        row_index_dfc_md++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_DFC_MI) {
        worksheet_write_string(worksheet_dfc_mi, row_index_dfc_mi, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_dfc_mi, row_index_dfc_mi, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_dfc_mi, row_index_dfc_mi, 2, accounts[i]->value, NULL);
        row_index_dfc_mi++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_DMPL) {
        worksheet_write_string(worksheet_dmpl, row_index_dmpl, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_dmpl, row_index_dmpl, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_dmpl, row_index_dmpl, 2, accounts[i]->value, NULL);
        row_index_dmpl++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_DVA) {
        worksheet_write_string(worksheet_dva, row_index_dva, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_dva, row_index_dva, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_dva, row_index_dva, 2, accounts[i]->value, NULL);
        row_index_dva++;
      }
    }
  }

  workbook_close(workbook);

  workbook = workbook_new("consolidate_df_export.xlsx");
  worksheet_bpa = workbook_add_worksheet(workbook, "BPA");

  worksheet_write_string(worksheet_bpa, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_bpa, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_bpa, 0, 2, "VL_CONTA", NULL);

  worksheet_bpp = workbook_add_worksheet(workbook, "BPP");

  worksheet_write_string(worksheet_bpp, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_bpp, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_bpp, 0, 2, "VL_CONTA", NULL);

  worksheet_dre = workbook_add_worksheet(workbook, "DRE");

  worksheet_write_string(worksheet_dre, 0, 0, "CD_CONTA", NULL);
  worksheet_write_string(worksheet_dre, 0, 1, "DS_CONTA", NULL);
  worksheet_write_string(worksheet_dre, 0, 2, "VL_CONTA", NULL);

  row_index_bpa = 1;
  row_index_bpp = 1;
  row_index_dre = 1;
  for (i = 0; i < nb_accounts; i++) {
    if (accounts[i]->financial_info_type == DFP_FINANCIAL_INFO_TYPE_CONSOLIDATED) {
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_BPA) {
        worksheet_write_string(worksheet_bpa, row_index_bpa, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_bpa, row_index_bpa, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_bpa, row_index_bpa, 2, accounts[i]->value, NULL);
        row_index_bpa++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_BPP) {
        worksheet_write_string(worksheet_bpp, row_index_bpp, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_bpp, row_index_bpp, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_bpp, row_index_bpp, 2, accounts[i]->value, NULL);
        row_index_bpp++;
      }
      if (accounts[i]->balance_type == DFP_BALANCE_TYPE_DRE) {
        worksheet_write_string(worksheet_dre, row_index_dre, 0, accounts[i]->number, NULL);
        worksheet_write_string(worksheet_dre, row_index_dre, 1, accounts[i]->description, NULL);
        worksheet_write_number(worksheet_dre, row_index_dre, 2, accounts[i]->value, NULL);
        row_index_dre++;
      }
    }
  }

  workbook_close(workbook);

  return 0;
}
