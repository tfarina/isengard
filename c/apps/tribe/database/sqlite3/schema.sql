/**
 * To recreate the database: sqlite3 abdb.sqlite3 < schema.sql
 */

BEGIN TRANSACTION;

/* Contacts table ----------------------------------------------------------- */

CREATE TABLE contacts (
  id       INTEGER PRIMARY KEY,
  fname    TEXT,
  lname    TEXT,
  mname    TEXT,
  nickname TEXT,
  email    TEXT
);

COMMIT;
