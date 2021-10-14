#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "b3company.h"
#include "fstrutils.h"
#include "test.h"

/**
 * 3.2 - REGISTRO - 01 - DADOS DAS EMPRESAS
 *
 * Descrição do Campo                 | Conteúdo | Tipo e Tamanho | Posição Inicial | Posição Final
 * 01 - Tipo de Registro              | Fixo '01'|     X(02)      |        01       |        02
 * 02 - Código da Empresa             |          |     X(04)      |        03       |        06
 * 03 - Denominação Social da Empresa |          |     X(60)      |        07       |        66
 * 04 - Nome Resumido da Empresa      |          |     X(12)      |        67       |        78
 * 05 - Reserva                       | Em branco|     X(142)     |        79       |        220
 */

static int
dump_b3_company(char const *filename)
{
  FILE *fp;
  char linebuf[BUFSIZ];
  char *company_code;
  char *company_name;
  char *company_short_name;

  company_t **companies; /* array of pointers to companies */
  unsigned nb_companies; /* number of companies */

  company_t *company;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return -1;
  }

  company_code = NULL;
  company_name = NULL;
  company_short_name = NULL;

  /* Loops through the file reading line by line. */
  while (fgets(linebuf, sizeof(linebuf), fp)) {
    if (str_startswith(linebuf, "01")) {
      company_code = str_substring(linebuf, 2, 6);
      company_code = str_strip(company_code);

      company_name = str_substring(linebuf, 6, 66);
      company_name = str_strip(company_name);

      company_short_name = str_substring(linebuf, 66, 78);
      company_short_name = str_strip(company_short_name);

      /* This is a small trick.
       * In the first pass 'companies' will be NULL, then this realloc call
       * will be equivalent to malloc(sizeof(company_t) * 1). */
      companies = realloc(companies, sizeof(company_t) * nb_companies + 1);
      if (companies == 0) {
	fputs("Out of memory\n", stderr);
        break;
      }

      company = malloc(sizeof(company_t));
      if (company == 0) {
        fputs("Out of memory\n", stderr);
        break;
      }

      company->code = company_code;
      company->name = company_name;
      company->short_name = company_short_name;

      companies[nb_companies++] = company;

      printf("%s %s %s\n", company_code, company_name, company_short_name);
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
    fputs("usage: b3company TITULOS_NEGOCIAVEIS.txt\n", stderr);
    return 1;
  }

  filename = f_strdup(argv[1]);

  if (!test_exists(filename)) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
  }

  retval = dump_b3_company(filename);
  if (retval < 0) {
    return 1;
  }

  free(filename);

  return 0;
}
