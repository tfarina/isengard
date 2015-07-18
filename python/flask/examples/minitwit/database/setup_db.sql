# SQL for setting up the database and user.

CREATE DATABASE IF NOT EXISTS minitwit;
GRANT ALL ON minitwit.* TO 'blaine'@'localhost' IDENTIFIED BY '19801219';
FLUSH PRIVILEGES;
