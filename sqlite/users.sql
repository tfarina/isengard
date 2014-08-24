PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;

CREATE TABLE user (
  uid INTEGER PRIMARY KEY,
  login TEXT UNIQUE,
  pw TEXT,
  email TEXT
);

INSERT INTO "user" VALUES (1, 'tff', 'abcdef12345', 'tff@gmail.com');
COMMIT;
