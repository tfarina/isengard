#!/bin/bash

DBNAME=todolist
DBUSER=kevin
DBPASS=chevrolet

q1="CREATE DATABASE IF NOT EXISTS todolist;"
q2="CREATE USER 'kevin'@'localhost' IDENTIFIED BY 'chevrolet';"
q3="GRANT ALL ON todolist.* TO 'kevin'@'localhost';"
q4="FLUSH PRIVILEGES;"
sql="${q1}${q2}${q3}${q4}"

mysql -u root -p -e "$sql"
