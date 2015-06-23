#!/bin/sh

# https://exploreflask.com/deployment.html

sudo kill -HUP `cat rocket.pid`
