#!/bin/bash

set -x -e

DBNAME=minitwit
DBUSER=blaine
DBPASS=19801219

mysql -u $DBUSER -p $DBNAME < schema.sql
