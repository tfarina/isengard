#!/bin/bash

# http://www.bluepiccadilly.com/2011/12/creating-mysql-database-and-user-command-line-and-bash-script-automate-process

set -x -e

DBNAME=bkms
DBUSER=seafowl
DBPASS=2Mnc7nXm

S1="CREATE DATABASE IF NOT EXISTS $DBNAME;"
S2="GRANT USAGE ON $DBNAME.* TO '$DBUSER'@localhost IDENTIFIED BY '$DBPASS';"
S3="GRANT ALL PRIVILEGES ON $DBNAME.* TO '$DBUSER'@localhost;"
S4="FLUSH PRIVILEGES;"
SQL="${S1}${S2}${S3}${S4}"

mysql -u root -p -e "$SQL"
