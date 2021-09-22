#include "ta.h"

int ta_strat_rsi(double rsi, int oversold, int overbought)
{
  /* If calculated rsi value FALLS below the oversold threshold,
   * signs point to an oversold stock; the price of the stock could
   * very well bounce back (upwards) and start to increase.
   */
  if ((int)rsi < oversold) {
    return TA_SIGNAL_BUY;
  }
  /* If calculated rsi value RISES above the overbought threshold,
   * signs point to an overbought stock; the price of the stock could
   * very well fall back (downwards) in the near future and start to decrease.
   */
  if ((int)rsi > overbought) {
    return TA_SIGNAL_SELL;
  }
  return TA_SIGNAL_HOLD;
}
