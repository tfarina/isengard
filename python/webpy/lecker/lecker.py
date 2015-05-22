#!/usr/bin/env python

# http://webpy.org/src/lecker

""" lec.ker - a simple del.icio.us clone """

import web
import model

urls = (
  '/', 'view',
  '/add', 'view',
  '/delete', 'delete',
  '/edit/([0-9]*)', 'edit',
  '/favicon.ico', 'icon',
)

### Templates
render = web.template.render('templates')

class view:
  def GET(self):
    bookmarks = model.get_bookmarks()
    return render.view(bookmarks)

  def POST(self):
    input = web.input()
    if input.title is None or input.url is None:
      bookmarks = model.get_bookmarks()
      return render.index(bookmarks)
    model.new_bookmark(input.title, input.url)
    raise web.seeother('/')


class delete:
  def GET(self):
    bookmarks = model.get_bookmarks()
    return render.delete(bookmarks)

  def POST(self):
    input = web.input()
    for id in input:
      model.del_bookmark(int(id))
    raise web.seeother('./#')


class edit:
  def GET(self, id):
    bookmark = model.get_bookmark(int(id))
    return render.edit(bookmark)

  def POST(self, id):
    input = web.input()
    model.edit_bookmark(id, input.title, input.url)
    raise web.seeother('/')


class icon:
  def GET(self):
    raise web.seeother("/static/favicon.ico")

app = web.application(urls, globals())

if __name__ == "__main__":
  app.run()
