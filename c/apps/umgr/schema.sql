/**
 * SQLITE DATABASE SCHEMA FOR THE 'umgr' APP.
 *
 * To recreate the database: sqlite3 abdb.sqlite3 < schema.sql
 */

PRAGMA foreign_keys=OFF;

BEGIN TRANSACTION;

/* Contacts table ----------------------------------------------------------- */

CREATE TABLE contacts (
  id INTEGER PRIMARY KEY,
  fname TEXT,
  lname TEXT,
  email TEXT
);

/* Initial Data ------------------------------------------------------------- */

/**
 * Populating with some values.
 */

INSERT INTO "contacts" VALUES (1, 'John', 'Doe', 'jdoe@yahoo.com');

COMMIT;
