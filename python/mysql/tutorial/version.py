#!/usr/bin/env python

import MySQLdb as mdb

DB_HOST = 'localhost'
DB_USER = 'vrossi'
DB_PASS = 'vr46'
DB_NAME = 'valentino'

connection = mdb.connect(DB_HOST, DB_USER, DB_PASS, DB_NAME)

with connection:
  cursor = connection.cursor()
  cursor.execute("SELECT VERSION()")

  version = cursor.fetchone()

  print "Database version: %s" % version
