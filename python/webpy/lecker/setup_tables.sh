#!/bin/bash

set -x -e

DBUSER=hannibal
DBNAME=lecker
DBPASS=19440807

mysql -u $DBUSER -p $DBNAME < schema.mysql
