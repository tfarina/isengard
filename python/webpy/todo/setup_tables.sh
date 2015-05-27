#!/bin/bash

set -x -e

DBNAME=todolist
DBUSER=kevin
DBPASS=chevrolet

mysql -u $DBUSER -p $DBNAME < schema.mysql
