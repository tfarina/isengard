#!/bin/bash

set -x -e

DBNAME=todolist
DBUSER=kevin
DBPASS=chevrolet

mysql -u root -p < setup_db.sql
