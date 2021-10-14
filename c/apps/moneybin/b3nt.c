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
 * 01 - Tipo de Registro              |Fixo '02' |     X(02)      |        01       |        02
 * 02 - Código de Negociação do Papel |          |     X(12)      |        03       |        14
 * 03 - Código da Empresa             |          |     X(04)      |        15       |        18
 * 04 - Código BDI                    |          |     N(03)      |        19       |        21
 * 05 - Descrição do CODBDI           |          |     X(60)      |        22       |        81
 * 06 - Código ISIN do Papel          |          |     X(12)      |        82       |        93
 * 07 - Código ISIN do Papel Objeto   |          |     X(12)      |        94       |       105
 * 08 - Número Distribuição do Papel  |          |     N(03)      |       106       |       108
 * 09 - Código do Mercado             |          |     N(03)      |       109       |       111
 * 10 - Descrição do Mercado          |          |     X(15)      |       112       |       126
 * 11 - Número de série p/ os mercados|          |     N(07)      |       127       |       133
 * de opções, termo secundário e      |          |                |                 |
 * futuro                             |          |                |                 |
 * 12 - Especificação do papel objeto |          |     X(10)      |       134       |       143               
 */

static int
dump_b3_stock_symbols(char const *filename)
{
  FILE *fp;
  char linebuf[BUFSIZ];
  char *symbol;
  char *company_code;
  char *bdi;
  char *bdi_description;
  char *isin; /* International Securities Identifying Number */
  char *isin_object;
  char *distrib_number;
  char *market_type;
  char *market_description;
  char *serial;
  char *type;
  char *due_year;
  char *due_month;
  char *due_day;
  char *price;
  char *option_style;
  char *currency_type;
  char *currency_type_description;
  char *protection;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return -1;
  }

  symbol = NULL;
  company_code = NULL;
  bdi = NULL;
  bdi_description = NULL;
  isin = NULL;
  isin_object = NULL;
  distrib_number = NULL;
  market_type = NULL;
  market_description = NULL;
  serial = NULL;
  type = NULL;
  due_year = NULL;
  due_month = NULL;
  due_day = NULL;
  price = NULL;
  option_style = NULL;
  currency_type = NULL;
  currency_type_description = NULL;
  protection = NULL;

  /* Loops through the file reading line by line. */
  while (fgets(linebuf, sizeof(linebuf), fp)) {
    if (str_startswith(linebuf, "00")) {
      /* read negotiable header */
    }
    else if (str_startswith(linebuf, "01")) {
      /* read negotiable company */
    }
    else if (str_startswith(linebuf, "02")) {
      symbol = str_substring(linebuf, 2, 14);
      symbol = str_strip(symbol);

      company_code = str_substring(linebuf, 14, 18);
      company_code = str_strip(company_code);

      bdi = str_substring(linebuf, 18, 21);
      bdi = str_strip(bdi);

      bdi_description = str_substring(linebuf, 21, 81);
      bdi_description = str_strip(bdi_description);

      isin = str_substring(linebuf, 81, 93);
      isin = str_strip(isin);

      isin_object = str_substring(linebuf, 93, 105);
      isin_object = str_strip(isin_object);

      distrib_number = str_substring(linebuf, 105, 108);
      distrib_number = str_strip(distrib_number);

      market_type = str_substring(linebuf, 108, 111);
      market_type = str_strip(market_type);

      market_description = str_substring(linebuf, 111, 126);
      market_description = str_strip(market_description);

      serial = str_substring(linebuf, 126, 133);
      serial = str_strip(serial);

      type = str_substring(linebuf, 133, 143);
      type = str_strip(type);

      due_year = str_substring(linebuf, 143, 147);
      due_year = str_strip(due_year);

      due_month = str_substring(linebuf, 148, 150);
      due_month = str_strip(due_month);

      due_day = str_substring(linebuf, 151, 153);
      due_day = str_strip(due_day);

      price = str_substring(linebuf, 153, 171);
      price = str_strip(price);

      option_style = str_substring(linebuf, 171, 172);
      option_style = str_strip(option_style);

      currency_type = str_substring(linebuf, 172, 175);
      currency_type = str_strip(currency_type);

      currency_type_description = str_substring(linebuf, 175, 190);
      currency_type_description = str_strip(currency_type_description);

      protection = str_substring(linebuf, 190, 193);
      protection = str_strip(protection);

      printf("%s\n", protection);
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
    fprintf(stderr, "usage: b3nt TITULOS_NEGOCIAVEIS.txt\n");
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
