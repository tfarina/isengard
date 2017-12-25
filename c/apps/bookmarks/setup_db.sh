#!/bin/bash

set -x -e

DBNAME=ctestdb
DBUSER=ken
DBPASS=194304

mysql -u root -p < setup_db.sql
