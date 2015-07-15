#!/bin/bash

set -x -e

DBNAME=minitwit
DBUSER=blaine
DBPASS=19801219

mysql -u root -p < setup_db.sql
