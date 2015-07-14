# http://agiliq.com/blog/2014/06/minimal-gunicorn-configuration/
bind='127.0.0.1:8000'
backlog=2048
pidfile='gunicorn.pid'
daemon=False
debug=False
workers=2
loglevel='info'
accesslog='/var/log/minitwit/gunicorn_access.log'
errorlog='/var/log/minitwit/gunicorn_error.log'
