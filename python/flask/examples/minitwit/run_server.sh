#!/bin/sh

# https://exploreflask.com/deployment.html

sudo gunicorn -c gunicorn.conf.py minitwit:app
