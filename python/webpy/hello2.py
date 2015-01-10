# http://webpy.org/docs/0.3/tutorial

import web

urls = (
  '/', 'index'
)


class index:
  def GET(self):
    return 'Hello, world!'


if __name__ == "__main__":
  app = web.application(urls, globals())
  app.run()
