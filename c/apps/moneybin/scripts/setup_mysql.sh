#!/bin/bash

cat <<MYSQLSCRIPT | mysql -u root -p
CREATE DATABASE IF NOT EXISTS moneybin;
GRANT ALL ON moneybin.* TO 'mbin'@'localhost' IDENTIFIED BY 'mbinpass';
FLUSH PRIVILEGES;
MYSQLSCRIPT
