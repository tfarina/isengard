# SQL for setting up the database and user.

CREATE DATABASE IF NOT EXISTS minitwit;
CREATE USER 'blaine'@'localhost' IDENTIFIED BY '19801219';
GRANT ALL ON minitwit.* TO 'blaine'@'localhost';
FLUSH PRIVILEGES;
