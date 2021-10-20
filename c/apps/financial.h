#ifndef FINANCIAL_H_
#define FINANCIAL_H_

/**
 * Computes the future value of an investment.
 *
 * @param pv   the present value
 * @param i    the interest rate compounded once per period
 * @param n    the number of periods (e.g., 10 years)
 *
 * @return the value at the end of 'n' periods
 */
double future_value(double pv, double i, double n);

/**
 * Computes the present value of an investment.
 *
 * @param fv   the future value
 * @param i    the interest rate compounded once per period
 * @param n    the number of periods (e.g., 10 years).
 */
double present_value(double fv, double i, double n);

/**
 * Compounding Annual Growth Rate (or CAGR).
 *
 * @param n the number of periods (e.g., 10 years).
 */
double growth_rate(double future_value, double present_value, double n);

/**
 * Returns the Current Ratio.
 */
double current_ratio(double current_assets, double current_liabilities);

/**
 * Returns the Basic Earnings Per Share.
 */
double eps_basic(double earnings, double shares_outstanding);

#endif  /* FINANCIAL_H_ */
