#!/usr/bin/env python

# Shows how to print column headers with the data from the database table.

import MySQLdb as mdb

connection = mdb.connect('localhost', 'pyuser', 'yellow46', 'python_tutorial')

with connection:
  cursor = connection.cursor()
  cursor.execute("SELECT * FROM writer")

  rows = cursor.fetchall()

  description = cursor.description

  print "%s %3s" % (description[0][0], description[1][0])

  for row in rows:
    print "%2s %3s" % row
