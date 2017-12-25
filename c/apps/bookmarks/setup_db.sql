# SQL for setting up the database and user.

CREATE DATABASE IF NOT EXISTS ctestdb;
GRANT ALL ON ctestdb.* TO 'ken'@'localhost' IDENTIFIED BY '194304';
FLUSH PRIVILEGES;
