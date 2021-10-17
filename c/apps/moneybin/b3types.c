#include "b3types.h"

security_bdi_t
get_security_bdi(int bdi)
{
  security_bdi_t type = SB_INVALID;

  switch (bdi) {
  case 2:
    type = SB_PADRAO;
    break;
  case 6:
    type = SB_CONCORDATARIA;
    break;
  case 10:
    type = SB_DIREITOS_E_RECIBOS;
    break;
  case 12:
    type = SB_FUNDOS_IMOBILIARIOS;
    break;
  case 14:
    type = SB_CERTIFIC_INVESTIMENTO;
    break;
  case 18:
    type = SB_OBRIGACOES;
    break;
  case 22:
    type = SB_BONUS_PRIVADOS;
    break;
  case 26:
    type = SB_APOLICES;
    break;
  case 32:
    type = SB_EXERCICIO_COMPRA_INDICE;
    break;
  case 33:
    type = SB_EXERCICIO_VENDA_INDICE;
    break;
  case 38:
    type = SB_EXERCICIO_COMPRA;
    break;
  case 42:
    type = SB_EXERCICIO_VENDA;
    break;
  case 46:
    type = SB_LEILAO_TITULOS_N_COTADOS;
    break;
  case 48:
    type = SB_LEILAO_PRIVATIZACAO;
    break;
  case 50:
    type = SB_LEILAO;
    break;
  case 51:
    type = SB_LEILAO_FINOR;
    break;
  case 52:
    type = SB_LEILAO_FINAM;
    break;
  case 53:
    type = SB_LEILAO_FISET;
    break;
  case 54:
    type = SB_LEILAO_ACOES_MORA;
    break;
  case 56:
    type = SB_VENDAS_ALVARA_JUDICIAL;
    break;
  case 58:
    type = SB_OUTROS;
    break;
  case 60:
    type = SB_PERMUTA;
    break;
  case 61:
    type = SB_META;
    break;
  case 62:
    type = SB_TERMO;
    break;
  case 66:
    type = SB_DEBENTUR_VENCIMENTO_ATE3;
    break;
  case 68:
    type = SB_DEBENTUR_VENCIMENTO_MAI3;
    break;
  case 70:
    type = SB_FUTURO_MOV_CONTINUA;
    break;
  case 71:
    type = SB_FUTURO_RETENCAO_GANHO;
    break;
  case 74:
    type = SB_OPCOES_COMPRA_INDICE;
    break;
  case 75:
    type = SB_OPCOES_VENDA_INDICE;
    break;
  case 78:
    type = SB_OPCOES_COMPRA;
    break;
  case 82:
    type = SB_OPCOES_VENDA;
    break;
  case 83:
    type = SB_DEBENTUR_NOTA_PROMISSORIA;
    break;
  case 96:
    type = SB_FRACIONARIO;
    break;
  case 99:
    type = SB_TOTAL_GERAL;
    break;
  }

  return type;
}
