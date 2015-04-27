#!/usr/bin/env python

import MySQLdb as mdb

connection = mdb.connect('localhost', 'vrossi', 'vr46', 'valentino')

with connection:
  cursor = connection.cursor()
  cursor.execute("SELECT VERSION()")

  version = cursor.fetchone()

  print "Database version: %s" % version
