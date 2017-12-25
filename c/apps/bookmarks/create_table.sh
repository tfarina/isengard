#!/bin/bash

set -x -e

DBNAME=ctestdb

mysql -u root -p $DBNAME < schema.mysql
