#include "ta.h"

int ta_strat_rsi(double rsi, int oversold, int overbought)
{
  if ((int)rsi < oversold) {
    return TA_SIGNAL_BUY;
  }
  if ((int)rsi > overbought) {
    return TA_SIGNAL_SELL;
  }
  return TA_SIGNAL_HOLD;
}
