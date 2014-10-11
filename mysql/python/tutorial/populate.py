#!/usr/bin/env python

import MySQLdb as mdb

connection = mdb.connect('localhost', 'pyuser', 'yellow46', 'python_tutorial')

with connection:
  cursor = connection.cursor()

  cursor.execute("DROP TABLE IF EXISTS writer")
  cursor.execute("CREATE TABLE writer(\
                  id INT PRIMARY KEY AUTO_INCREMENT, \
                  name VARCHAR(25)) ENGINE=INNODB")
  cursor.execute("INSERT INTO writer(name) VALUES('Jack London')")
  cursor.execute("INSERT INTO writer(name) VALUES('Honore de Balzac')")
  cursor.execute("INSERT INTO writer(name) VALUES('Lion Feuchtwanger')")
  cursor.execute("INSERT INTO writer(name) VALUES('Emile Zola')")
  cursor.execute("INSERT INTO writer(name) VALUES('Truman Capote')")
  cursor.execute("INSERT INTO writer(name) VALUES('Terry Pratchett')")
