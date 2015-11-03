#!/usr/bin/env python

from flask import Flask

app = Flask(__name__)


@app.route("/")
def index():
  return "Index page!"


@app.route("/hello")
def hello():
  return "Hello"


if __name__ == "__main__":
  app.run(host='0.0.0.0')
