#!/bin/bash

set -x -e

DBNAME=pdns

mysql -u root -p $DBNAME < schema.mysql.sql
