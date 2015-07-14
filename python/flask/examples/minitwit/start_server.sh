#!/bin/bash

# https://exploreflask.com/deployment.html

exec /usr/local/bin/gunicorn -c gunicorn.conf.py minitwit:app
