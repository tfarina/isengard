#!/usr/bin/env python

import web

db = web.database(dbn='mysql', db='python_tutorial', user='pyuser', passwd='yellow46')

for row in db.select('writer'):
  print row

for row in db.select('writer'):
  print row["id"], row["name"]
