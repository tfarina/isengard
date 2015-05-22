#!/bin/bash

set -x -e

DBNAME=lecker
DBUSER=hannibal
DBPASS=19440807

mysql -u hannibal -p lecker < schema.mysql
