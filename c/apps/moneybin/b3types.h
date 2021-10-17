#ifndef _B3TYPES_H_INCLUDED_
#define _B3TYPES_H_INCLUDED_ 1

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

/**
 * TABELA DE CODBDI - RELAÇÃO DOS VALORES PARA CÓDIGOS DE BDI
 */
typedef enum security_bdi_e {
  SB_INVALID = -1, /** Valor invalido */
  SB_PADRAO = 2, /** Lote Padrao */
  SB_CONCORDATARIA = 6, /** Concordataria */
  SB_DIREITOS_E_RECIBOS = 10, /** Direitos e recibos */
  SB_FUNDOS_IMOBILIARIOS = 12, /** Fundos imobiliarios */
  SB_CERTIFIC_INVESTIMENTO = 14, /** Certific. investimento / debentures / titulo divida publica */
  SB_OBRIGACOES = 18, /** Obrigacoes */
  SB_BONUS_PRIVADOS = 22, /** Bonus (privados) */
  SB_APOLICES = 26, /** Apolices / bonus / titulos publicos */
  SB_EXERCICIO_COMPRA_INDICE = 32, /** Exercicio de opcoes de compra de indice */
  SB_EXERCICIO_VENDA_INDICE = 33, /** Exercicio de opcoes de venda de indice */
  SB_EXERCICIO_COMPRA = 38, /** Exercicio de opcoes de compra */
  SB_EXERCICIO_VENDA = 42, /** Exercicio de opcoes de venda */
  SB_LEILAO_TITULOS_N_COTADOS = 46, /** Leilao de titulos nao cotados */
  SB_LEILAO_PRIVATIZACAO = 48, /** Leilao de privatizacao */
  SB_LEILAO = 50, /** Leilao */
  SB_LEILAO_FINOR = 51, /** Leilao FINOR */
  SB_LEILAO_FINAM = 52, /** Leilao FINAM */
  SB_LEILAO_FISET = 53, /** Leilao FISET */
  SB_LEILAO_ACOES_MORA = 54, /** Leilao de acoes em mora */
  SB_VENDAS_ALVARA_JUDICIAL = 56, /** Vendas por alvara judicial */
  SB_OUTROS = 58, /** Outros */
  SB_PERMUTA = 60, /** Permuta por acoes */
  SB_META = 61, /** Meta */
  SB_TERMO = 62, /** Termo */
  SB_DEBENTUR_VENCIMENTO_ATE3 = 66, /** Debentures com data de vencimento ate 3 anos */
  SB_DEBENTUR_VENCIMENTO_MAI3 = 68, /** Debentures com data de vencimento maior que 3 anos */
  SB_FUTURO_MOV_CONTINUA = 70, /** Futuro com movimentacao continua */
  SB_FUTURO_RETENCAO_GANHO = 71, /** Futuro com retencao de ganho */
  SB_OPCOES_COMPRA_INDICE = 74, /** Opcoes de compra de indices */
  SB_OPCOES_VENDA_INDICE = 75, /** Opcoes de venda de indices */
  SB_OPCOES_COMPRA = 78, /** Opcoes de compra */
  SB_OPCOES_VENDA = 82, /** Opcoes de venda */
  SB_DEBENTUR_NOTA_PROMISSORIA = 83, /** Debentures e notas promissorias */
  SB_FRACIONARIO = 96, /** Fracionario */
  SB_TOTAL_GERAL = 99 /** Total geral */
} security_bdi_t;

security_bdi_t get_security_bdi(int bdi);

#endif  /* !defined(_B3TYPES_H_INCLUDED_) */
