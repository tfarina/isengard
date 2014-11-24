#!/bin/bash

DBNAME=blog
DBUSER=ryan
DBPASS=31newman

q1="CREATE DATABASE IF NOT EXISTS blog;"
q2="CREATE USER 'ryan'@'localhost' IDENTIFIED BY '31newman';"
q3="GRANT ALL ON blog.* TO 'ryan'@'localhost';"
q4="FLUSH PRIVILEGES;"
sql="${q1}${q2}${q3}${q4}"

mysql -u root -p -e "$sql"
