#!/bin/sh

# https://exploreflask.com/deployment.html

sudo kill -HUP `cat /tmp/uwsgi-minitwit.pid`
