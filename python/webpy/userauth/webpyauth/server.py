#!/usr/bin/env python

import web
from web.contrib.auth import DBAuth

urls = (
    '/', 'index',
    '/login', 'login',
    'somePage', 'somePage',
)

app = web.application(urls, locals())
db = web.database(dbn='mysql', db='webpyauth', user='joe', passwd='camaro')

render = web.template.render('templates')
settings = dict (
  template_login = render.login
)

mysession = web.session.Session(app, web.session.DiskStore('sessions'))
auth = DBAuth(app, db, **settings)

class index:
    def GET(self):
        return render.index()

class somePage:
  @auth.protected()
  def GET(self):
    return "some blabal"


if __name__ == "__main__":
    app.run()
