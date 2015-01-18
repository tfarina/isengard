import web

db = web.database(dbn='mysql', db='lecker', user='hannibal', passwd='19440807')

def get_bookmarks():
  return db.select('bookmarks', order='created desc')

def new_bookmark(title, url):
  db.insert('bookmarks', title=title, url=url)

def del_bookmark(id):
  db.delete('bookmarks', where="id=$id", vars=locals())

def get_bookmark(id):
  try:
    return db.select('bookmarks', where='id=$id', vars=locals())[0]
  except IndexError:
    return None
