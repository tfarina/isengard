#!/usr/bin/env python

import MySQLdb as mdb

connection = mdb.connect('localhost', 'pyuser', 'yellow46', 'python_tutorial')

with connection:
  cursor = connection.cursor(mdb.cursors.DictCursor)
  cursor.execute("SELECT * FROM writer")

  rows = cursor.fetchall()

  for row in rows:
    print row

  # With DictCursor we can refer to data by their column names.
  for row in rows:
    print row["id"], row["name"]
