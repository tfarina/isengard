#!/bin/bash

set -x -e

DBNAME=minitwit

mysql -u root -p $DBNAME < database/schema.sql
