/*
 * SQLITE DATABASE SCHEMA FOR THE cli DATABASE.
 *
 * To recreate the database: sqlite3 users.db < sqlite-schema.sql
 *
 */


PRAGMA foreign_keys=OFF;

BEGIN TRANSACTION;

/* User table --------------------------------------------------------------- */

CREATE TABLE user (
  uid INTEGER PRIMARY KEY,
  login TEXT UNIQUE,
  pw TEXT,
  email TEXT
);

/* Initial Data ------------------------------------------------------------- */

/*
 * Populating with standard values.
 */

INSERT INTO "user" VALUES (1, 'tff', 'abcdef12345', 'tff@gmail.com');

COMMIT;
