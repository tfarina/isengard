#!/usr/bin/env python

from flask import Flask, request


app = Flask(__name__)


@app.route("/")
def index():
  ip = request.remote_addr
  return request.environ['REMOTE_ADDR']


if __name__ == "__main__":
  app.run(host='0.0.0.0')
