#!/bin/bash

set -x -e

DBNAME=pdns
DBUSER=hubert
DBPASS=272011

mysql -u root -p < setup_db.sql
