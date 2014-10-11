#!/usr/bin/env python

import MySQLdb as mdb

connection = mdb.connect('localhost', 'pyuser', 'yellow46', 'python_tutorial')

with connection:
  cursor = connection.cursor()
  cursor.execute("SELECT * FROM writer")

  for i in range(cursor.rowcount):
    row = cursor.fetchone()
    print row[0], row[1]
