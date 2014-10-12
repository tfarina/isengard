#!/bin/bash

EXPECTED_ARGS=3
E_BADARGS=65

q1="CREATE DATABASE IF NOT EXISTS $3;"
q2="CREATE USER $1@'localhost' IDENTIFIED BY $2;"
q3="GRANT ALL ON $3.* TO $1@localhost;"
q4="FLUSH PRIVILEGES;"
sql="${q1}${q2}${q3}${q4}"

if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: $0 dbuser dbpass dbname"
  exit $E_BADARGS
fi

mysql -u root -p -e "$sql"
