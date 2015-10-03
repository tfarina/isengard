import os
from shop import app


def runserver():
  port = int(os.environ.get('PORT', 8001))
  app.run(host='0.0.0.0', port=port)


if __name__ == '__main__':
  runserver()
