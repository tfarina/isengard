# SQL for setting up the database and user.

CREATE DATABASE IF NOT EXISTS todolist;
CREATE USER 'kevin'@'localhost' IDENTIFIED BY 'chevrolet';
GRANT ALL ON todolist.* TO 'kevin'@'localhost';
FLUSH PRIVILEGES;
