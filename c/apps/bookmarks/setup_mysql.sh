#!/bin/bash

cat <<MYSQLSCRIPT | mysql -u root -p
CREATE DATABASE IF NOT EXISTS classicmodels;
GRANT ALL ON classicmodels.* TO 'testuser'@'localhost' IDENTIFIED BY 'testpass';
FLUSH PRIVILEGES;
MYSQLSCRIPT
