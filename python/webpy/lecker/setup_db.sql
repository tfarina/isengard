# SQL for setting up the database and user.

CREATE DATABASE IF NOT EXISTS lecker;
CREATE USER 'hannibal'@'localhost' IDENTIFIED BY '19440807';
GRANT ALL ON lecker.* TO 'hannibal'@'localhost';
FLUSH PRIVILEGES;
