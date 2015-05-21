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
)

### Templates
render = web.template.render('templates')

class view:
  form = web.form.Form(
    web.form.Textbox('title', web.form.notnull, description="Title:"),
    web.form.Textbox('url', web.form.notnull, description="Url:"),
    web.form.Button('Add'),
  )

  def GET(self):
    bookmarks = model.get_bookmarks()
    form = self.form
    return render.view(bookmarks, form)

  def POST(self):
    form = self.form
    if not form.validates():
      bookmarks = model.get_bookmarks()
      return render.index(bookmarks, form)
    model.new_bookmark(form.d.title, form.d.url)
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

app = web.application(urls, globals())

if __name__ == "__main__":
  app.run()
