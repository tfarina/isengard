#!/bin/bash

DBNAME=lecker
DBUSER=hannibal
DBPASS=19440807

q1="CREATE DATABASE IF NOT EXISTS lecker;"
q2="CREATE USER 'hannibal'@'localhost' IDENTIFIED BY '19440807';"
q3="GRANT ALL ON lecker.* TO 'hannibal'@'localhost';"
q4="FLUSH PRIVILEGES;"
sql="${q1}${q2}${q3}${q4}"

mysql -u root -p -e "$sql"
