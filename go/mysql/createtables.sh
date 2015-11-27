#!/bin/bash

set -x -e

DBUSER=nash
DBNAME=go_tutorial
DBPASS=virginia

mysql -u $DBUSER -p $DBNAME < schema.sql
