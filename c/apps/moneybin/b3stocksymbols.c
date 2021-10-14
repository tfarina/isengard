#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fstrutils.h"
#include "test.h"

/**
 * 3.3 - REGISTRO - 02 - DADOS DOS TÍTULOS NEGOCIÁVEIS
 *
 * Descrição do Campo                 | Conteúdo | Tipo e Tamanho | Posição Inicial | Posição Final
 * 02 - Código de Negociação do Papel |          |     X(12)      |        03       |        14
 * 03 - Código da Empresa             |          |     X(04)      |        15       |        18
 * 09 - Código do Mercado             |          |     N(03)      |       109       |       111
 */

/**
 * TABELA DE TPMERC - RELAÇÃO DOS VALORES PARA TIPO DE MERCADO
 */
typedef enum market_type_e {
  /**
   * 010 VISTA
   */
  MT_EQUITY = 10,

  /**
   * 012 EXERCÍCIO DE OPÇÕES DE COMPRA
   */
  MT_CALL_EXERCISE = 12,

  /**
   * 013 EXERCÍCIO DE OPÇÕES DE VENDA
   */
  MT_PUT_EXERCISE = 13,

  /**
   * 017 LEILÃO
   */
  MT_AUCTION = 17,

  /**
   * 020 FRACIONÁRIO
   */
  MT_FRACTIONARY = 20,

  /**
   * 030 TERMO
   */
  MT_TERM = 30,

  /**
   * 050 FUTURO COM RETENÇÃO DE GANHO
   */
  MT_FUTURE_GAIN_RETENTION = 50,

  /**
   * 060 FUTURO COM MOVIMENTAÇÃO CONTÍNUA
   */
  MT_FUTURE_CONTINUOS_MOVING = 60,

  /**
   * 070 OPÇÕES DE COMPRA
   */
  MT_CALL = 70,

  /**
   * 080 OPÇÕES DE VENDA
   */
  MT_PUT = 80
} market_type_t;

static int
dump_b3_stock_symbols(char const *filename)
{
  FILE *fp;
  char linebuf[BUFSIZ];
  char *market_type;
  char *symbol;
  char *company_code;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return -1;
  }

  market_type = NULL;
  symbol = NULL;
  company_code = NULL;

  /* Loops through the file reading line by line. */
  while (fgets(linebuf, sizeof(linebuf), fp)) {
    market_type = str_substring(linebuf, 108, 111);

    if (str_startswith(market_type, "010")) {
      symbol = str_substring(linebuf, 2, 14);
      symbol = str_strip(symbol);

      company_code = str_substring(linebuf, 14, 18);
      company_code = str_strip(company_code);

      printf("%s\n", symbol);
    }
  }

  fclose(fp);

  return 0;
}

int
main(int argc, char **argv)
{
  int retval;
  char *filename;

  if (argc != 2) {
    fprintf(stderr, "usage: b3stocksymbols TITULOS_NEGOCIAVEIS.txt\n");
    return 1;
  }

  filename = f_strdup(argv[1]);

  if (!test_exists(filename)) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
  }

  retval = dump_b3_stock_symbols(filename);
  if (retval < 0) {
    return 1;
  }

  free(filename);

  return 0;
}
