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

#endif  /* !defined(_B3TYPES_H_INCLUDED_) */
