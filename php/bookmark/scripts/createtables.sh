#!/bin/bash

set -x -e

DBUSER=seafowl
DBNAME=bkms
DBPASS=2Mnc7nXm

mysql -u $DBUSER -p $DBNAME < schema.sql
