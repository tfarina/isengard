# SQL for setting up the database and user.

CREATE DATABASE IF NOT EXISTS lecker;
GRANT ALL ON lecker.* TO 'hannibal'@'localhost' IDENTIFIED BY '19440807';
FLUSH PRIVILEGES;
