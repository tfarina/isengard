#!/bin/bash

# http://www.bluepiccadilly.com/2011/12/creating-mysql-database-and-user-command-line-and-bash-script-automate-process

set -x -e

DBNAME=go_tutorial
DBUSER=nash
DBPASS=virginia

s1="CREATE DATABASE IF NOT EXISTS $DBNAME;"
s2="CREATE USER '$DBUSER'@'localhost' IDENTIFIED BY '$DBPASS';"
s3="GRANT ALL ON $DBNAME* TO '$DBUSER'@'localhost';"
s4="FLUSH PRIVILEGES;"
sql="${s1}${s2}${s3}${s4}"

mysql -u root -p -e "$sql"
