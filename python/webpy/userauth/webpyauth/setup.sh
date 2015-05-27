#!/bin/bash

DBNAME=todolist
DBUSER=kevin
DBPASS=chevrolet

q1="CREATE DATABASE IF NOT EXISTS webpyauth;"
q2="CREATE USER 'joe'@'localhost' IDENTIFIED BY 'camaro';"
q3="GRANT ALL ON webpyauth.* TO 'joe'@'localhost';"
q4="FLUSH PRIVILEGES;"
sql="${q1}${q2}${q3}${q4}"

mysql -u root -p -e "$sql"
