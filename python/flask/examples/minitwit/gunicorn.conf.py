# http://agiliq.com/blog/2014/06/minimal-gunicorn-configuration/
bind='127.0.0.1:8000'
pidfile='gunicorn.pid'
daemon=True
accesslog='gunicorn_access.log'
errorlog='gunicorn_error.log'
