#!/bin/sh

# https://exploreflask.com/deployment.html

sudo kill `cat gunicorn.pid`
