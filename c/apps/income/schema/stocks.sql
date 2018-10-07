CREATE TABLE `exchange` (
  `id` int NOT NULL AUTO_INCREMENT,
  `abbrev` varchar(32) NOT NULL,
  `name` varchar(255) NOT NULL,
  `city` varchar(255) NULL,
  `country` varchar(255) NULL,
  `currency` varchar(64) NULL,
  `timezone_offset` time NULL,
  `created_at` datetime NOT NULL,
  `updated_at` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

insert into exchange (abbrev, name, city, country, currency, created_date, last_updated_date) values ('B3', 'Brasil, Bolsa, Balcão', 'São Paulo', 'Brazil', 'BRL', NOW(), NOW());

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
