/**
 * SQLITE DATABASE SCHEMA FOR THE 'ADDRESS BOOK' APP.
 *
 * To recreate the database: sqlite3 abdb.sqlite3 < schema.sql
 */

PRAGMA foreign_keys=OFF;

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

/* Email address table ----------------------------------------------------- */

/*
 * Date and time information is represented using ISO 8601 format and is
 * always in UTC time.
 */
CREATE TABLE EMAIL_ADDRESS (
  emailID              INTEGER PRIMARY KEY,
  contactID            INTEGER,
  isPrimary            INTEGER,
  orderingIndex        INTEGER,
  label                TEXT,
  address              TEXT,
  createdDateTime      TEXT,
  lastModifiedDateTime TEXT
);

/* Phone number table ----------------------------------------------------- */

/*
 * Date and time information is represented using ISO 8601 format and is
 * always in UTC time.
 */
CREATE TABLE PHONE_NUMBER (
  phoneID              INTEGER PRIMARY KEY,
  contactID            INTEGER,
  isPrimary            INTEGER,
  orderingIndex        INTEGER,
  label                TEXT,
  areaCode             TEXT,
  countryCode          TEXT,
  extension            TEXT,
  fullNumber           TEXT,
  createdDateTime      TEXT,
  lastModifiedDateTime TEXT
);

COMMIT;
