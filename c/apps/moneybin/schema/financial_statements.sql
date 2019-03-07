CREATE TABLE `income_statement` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `stock_id` INT NOT NULL,
  `revenue` BIGINT,
  `cost_of_goods_sold` BIGINT,
  `gross_profit` BIGINT,
  `net_income` BIGINT,
  `year` YEAR NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB;

CREATE TABLE `balance_sheet` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `stock_id` INT NOT NULL,
  `current_assets` BIGINT,
  `total_assets` BIGINT,
  `current_liabilities` BIGINT,
  `total_liabilities` BIGINT,
  `shareholders_equity` BIGINT,
  `year` YEAR NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB;

CREATE TABLE `cashflow_statement` (
  `currencyCode` CHAR(3),
  `netIncome` BIGINT,
  `depreciation` BIGINT,
  `netIncomeAdjustments` BIGINT,
  `changeReceivables` BIGINT,
  `changeLiabilities` BIGINT,
  `changeInventories` BIGINT,
  `cashFlowsOtherOperating` BIGINT,
  `cashFlowsTotalOperating` BIGINT,
  `capitalExpenditures` BIGINT,
  `investments` BIGINT,
  `cashFlowsOtherInvesting` BIGINT,
  `cashFlowsTotalInvesting` BIGINT,
  `dividends` BIGINT,
  `stockSalePurchase` BIGINT,
  `netBorrowings` BIGINT,
  `cashFlowsOtherFinancing` BIGINT,
  `cashFlowsTotalFinancing` BIGINT,
  `exchangeRateChanges` BIGINT,
  `cashAndCashEquivalentsChanges` BIGINT,
);

CREATE TABLE `dividends` (
  `symbol`,
  `type`,
  `declaredDate`,
  `recordDate`,
  `exDate`,
  `payableDate`,
  `amount`,
);
