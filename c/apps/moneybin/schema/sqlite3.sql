CREATE TABLE exchange (
  id INTEGER,
  name TEXT NOT NULL,
  currency,
  code TEXT NOT NULL UNIQUE,
  PRIMARY KEY(id)
);

CREATE TABLE company (
  id INTEGER,
  name TEXT NOT NULL,
  industry TEXT,
  sector TEXT,
  hq_location TEXT,
  securityID INTEGER,
  PRIMARY KEY(id),
  FOREIGN KEY(securityID) REFERENCES security (id)
);

CREATE TABLE security (
  id INTEGER,
  ticker TEXT NOT NULL UNIQUE,
  name TEXT NOT NULL,
  companyID INTEGER,
  exchangeID INTEGER,
  FOREIGN KEY(companyID) REFERENCES company (id),
  FOREIGN KEY(exchangeID) REFERENCES exchange (id)
);

CREATE TABLE security_price (
  id INTEGER,
  date TEXT NOT NULL,
  open REAL NOT NULL,
  high REAL NOT NULL,
  low REAL NOT NULL,
  close REAL NOT NULL,
  adjClose REAL NOT NULL,
  volume INTEGER,
  PRIMARY KEY(id)
);
