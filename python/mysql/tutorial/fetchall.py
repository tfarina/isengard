#!/usr/bin/env python

import MySQLdb as mdb

connection = mdb.connect('localhost', 'pyuser', 'yellow46', 'python_tutorial')

with connection:
  cursor = connection.cursor()
  cursor.execute("SELECT * FROM writer")

  rows = cursor.fetchall()

  for row in rows:
    print row
