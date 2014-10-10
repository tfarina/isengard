#!/usr/bin/env python

import MySQLdb

connection = MySQLdb.connect('localhost', 'vrossi', 'vr46', 'valentino')

with connection:
  cursor = connection.cursor()
  cursor.execute("SELECT * FROM user")

  rows = cursor.fetchall()

  for row in rows:
    print row
