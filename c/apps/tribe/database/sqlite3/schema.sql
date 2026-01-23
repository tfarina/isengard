/*
 * SQLite schema snapshot
 *
 * This file represents the CURRENT database schema as implemented
 * by the application at a given point in time.
 *
 * The authoritative schema definition and all schema migrations
 * live in the application source code.
 *
 * This file is intended for:
 *   - developer reference
 *   - inspecting the schema layout
 *   - creating a fresh database for development or testing
 *
 * It is NOT intended to track schema history or migrations.
 *
 * To create a NEW database using this schema snapshot, run the command:
 *
 *   sqlite3 abdb.sqlite3 < schema.sql
 *
 * Existing databases must be upgraded using the application's
 * built-in schema migration logic.
 */

BEGIN TRANSACTION;

/* Contacts table ----------------------------------------------------------- */

CREATE TABLE contacts (
  id       INTEGER PRIMARY KEY,
  fname    TEXT,
  lname    TEXT,
  email    TEXT
);

COMMIT;
