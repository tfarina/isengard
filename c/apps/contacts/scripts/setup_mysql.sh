#!/bin/bash

cat <<MYSQLSCRIPT | mysql -u root -p
CREATE DATABASE IF NOT EXISTS sab;
GRANT ALL ON sab.* TO 'testuser'@'localhost' IDENTIFIED BY 'testpass';
FLUSH PRIVILEGES;
MYSQLSCRIPT
