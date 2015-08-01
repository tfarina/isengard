# SQL for setting up the database and user.

CREATE DATABASE IF NOT EXISTS pdns;
GRANT ALL ON pdns.* TO 'hubert'@'localhost' IDENTIFIED BY '272011';
FLUSH PRIVILEGES;
