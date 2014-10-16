#!/bin/bash

DBNAME=go_tutorial
DBUSER=nash
DBPASS=virginia

q1="CREATE DATABASE IF NOT EXISTS go_tutorial;"
q2="CREATE USER 'nash'@'localhost' IDENTIFIED BY 'virginia';"
q3="GRANT ALL ON go_tutorial.* TO 'nash'@'localhost';"
q4="FLUSH PRIVILEGES;"
sql="${q1}${q2}${q3}${q4}"

mysql -u root -p -e "$sql"
