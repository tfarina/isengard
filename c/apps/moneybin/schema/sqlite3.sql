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
