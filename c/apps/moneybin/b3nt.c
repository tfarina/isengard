#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "b3company.h"
#include "b3security.h"
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
read_negotiable_company(char *linebuf, company_t *company)
{
  char *company_code;
  char *company_name;
  char *company_short_name;

  company_code = NULL;
  company_name = NULL;
  company_short_name = NULL;

  company_code = str_substring(linebuf, 2, 6);
  company_code = str_strip(company_code);

  company_name = str_substring(linebuf, 6, 66);
  company_name = str_strip(company_name);

  company_short_name = str_substring(linebuf, 66, 78);
  company_short_name = str_strip(company_short_name);

  company->code = company_code;
  company->name = company_name;
  company->short_name = company_short_name;

#if 0
  printf("%s %s %s\n", company_code, company_name, company_short_name);
#endif

  return 0;
}

static int
parse_b3_negotiable_securities(char const *filename)
{
  FILE *fp;
  char linebuf[BUFSIZ];
  company_t *company;
  security_t *security;
  char *symbol;
  char *company_code;
  char *bdi;
  char *bdi_description;
  char *isin; /* International Securities Identifying Number */
  char *isin_object;
  char *distribution_number;
  char *market_type;
  char *market_description;
  char *serial_number;
  char *security_type;
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
  distribution_number = NULL;
  market_type = NULL;
  market_description = NULL;
  serial_number = NULL;
  security_type = NULL;
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
      company = malloc(sizeof(company_t));
      if (company == 0) {
        fputs("Out of memory\n", stderr);
        break;
      }

      read_negotiable_company(linebuf, company);
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

      distribution_number = str_substring(linebuf, 105, 108);
      distribution_number = str_strip(distribution_number);

      market_type = str_substring(linebuf, 108, 111);
      market_type = str_strip(market_type);

      market_description = str_substring(linebuf, 111, 126);
      market_description = str_strip(market_description);

      serial_number = str_substring(linebuf, 126, 133);
      serial_number = str_strip(serial_number);

      security_type = str_substring(linebuf, 133, 143);
      security_type = str_strip(security_type);

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

      security = malloc(sizeof(security_t));
      if (security == 0) {
        fputs("Out of memory\n", stderr);
	break;
      }

      security->symbol = symbol;
      security->company_code = company_code;
      security->bdi = bdi;
      security->bdi_description = bdi_description;
      security->isin = isin;
      security->isin_object = isin_object;
      security->distribution_number = distribution_number;
      security->market_type = market_type;
      security->market_description = market_description;
      security->serial_number = serial_number;
      security->security_type = security_type;
      security->due_year = due_year;
      security->due_month = due_month;
      security->due_day = due_day;
      security->price = price;
      security->option_style = option_style;
      security->currency_type = currency_type;
      security->currency_type_description = currency_type_description;
      security->protection = protection;

      printf("%s %s %s %s %s %s %s %s %s %s %s %.4s-%.2s-%.2s %s %s %s %s %s\n",
	     symbol, company_code, bdi, bdi_description,
	     isin, isin_object, distribution_number, market_type, market_description,
	     serial_number, security_type, due_year, due_month, due_day, price, option_style,
	     currency_type, currency_type_description, protection);
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

  retval = parse_b3_negotiable_securities(filename);
  if (retval < 0) {
    return 1;
  }

  free(filename);

  return 0;
}
