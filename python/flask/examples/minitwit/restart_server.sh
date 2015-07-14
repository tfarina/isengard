#!/bin/sh

# https://exploreflask.com/deployment.html

sudo kill -HUP `cat /var/run/gunicorn.pid`
