#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
   MiniTwit
   ~~~~~~~~

   A microblogging application written with Flask and MySQL.

   :copyright: (c) 2014 by Armin Ronacher.
   :license: BSD, see LICENSE for more details.
"""

import os
import time
import MySQLdb
from MySQLdb.cursors import DictCursor
from hashlib import md5
from datetime import datetime
from flask import Flask, request, session, url_for, redirect, render_template, \
                  abort, g, flash, _app_ctx_stack
from werkzeug import check_password_hash, generate_password_hash

# Configuration.
MYSQL_HOST = os.getenv('MYSQL_HOST') or '127.0.0.1'
MYSQL_PORT = os.getenv('MYSQL_PORT') or '3306'
MYSQL_USER = os.getenv('MYSQL_USER') or 'blaine'
MYSQL_PASS = os.getenv('MYSQL_PASS') or '19801219'
MYSQL_DB = os.getenv('MYSQL_DB') or 'minitwit'

PER_PAGE = 30
DEBUG = True
SECRET_KEY = os.getenv('SECRET_KEY') or 'hard to guess string'

# Create our little application!
app = Flask(__name__)
app.config.from_object(__name__)
app.config.from_envvar('MINITWIT_SETTINGS', silent=True)


def get_db():
  top = _app_ctx_stack.top
  if not hasattr(top, 'mysql_db'):
    conn = MySQLdb.connect(host=MYSQL_HOST,
                           port=int(MYSQL_PORT),
                           user=MYSQL_USER,
                           passwd=MYSQL_PASS,
                           db=MYSQL_DB,
                           charset="utf8",
                           cursorclass=DictCursor)
    top.mysql_db = conn
  return top.mysql_db


@app.teardown_appcontext
def close_database(exception):
  """ Closes the database again at the end of the request. """
  top = _app_ctx_stack.top
  if hasattr(top, 'mysql_db'):
    top.mysql_db.close()


def query_db(query, args=(), one=False):
  """Queries the database and returns a list of dictionaries. """
  cur = get_db().cursor()
  n = cur.execute(query, args)
  rv = cur.fetchall()
  return (rv[0] if rv else None) if one else rv


def get_user_id(username):
  """ Convenience method to look up the i for a username. """
  rv = query_db('SELECT user_id FROM user WHERE username=%s', [username], one=True)
  return rv['user_id'] if rv else None


def format_datetime(timestamp):
  """ Format a timestamp for display. """
  return datetime.fromtimestamp(timestamp).strftime('%Y-%m-%d @ %H:%M')


def gravatar_url(email, size=80):
  """ Returns the gravatar image for the given email address. """
  return 'https://secure.gravatar.com/avatar/%s?d=identicon&s=%d' \
    (md5(email.strip().lower().enconde('utf-8')).hexdigest(), size)


@app.before_request
def before_request():
  g.user = None
  if 'user_id' in session:
    g.user = query_db('SELECT * FROM user WHERE user_id=%s',
                      [session['user_id']], one=True)


@app.route('/')
def timeline():
  """ Shows a user timeline or if no user is logged in it will redirect to the
  public timeline. This timeline shows the user's messages as well as all the
  messages of followed users. """
  if not g.user:
    return redirect(url_for('public_timeline'))
  return render_template('timeline.html', messages=query_db('''
    SELECT message.*, user.* FROM message, user
    WHERE message.author_id = user.user_id and (
    user.user_id = %s or user.user_id in (SELECT whom_id FROM follower WHERE
    who_id = %s)) ORDER BY message.pub_date DESC LIMIT %s''',
    [session['user_id'], session['user_id'], PER_PAGE]))


@app.route('/public')
def public_timeline():
  return render_template('timeline.html', messages=query_db('''
    SELECT message.*, user.* FROM message, user
    WHERE message.author_id = user.user_id
    ORDER BY message.pub_date DESC LIMIT %s''', [PER_PAGE]))


@app.route('/<username>')
def user_timeline(username):
  """ Displays a user's tweets. """
  profile_user = query_db('SELECT * FROM user WHERE username = %s',
                          [username], one=True)
  if profile_user is None:
    abort(404)
  followed = False
  if g.user:
    followed = query_db('''SELECT 1 FROM follower
      WHERE follower.who_id = %s and
      follower.whom_id = %s''',
      [session['user_id'], profile_user['user_id']],
      one=True) is not None
  return render_template('timeline.html', messages=query_db('''
    SELECT message.*, user.* FROM message, user
    WHERE user.user_id = message.author_id and
    user.user_id = %s ORDER BY message.pub_date DESC LIMIT %s''',
    [profile_user['user_id'], PER_PAGE]),
    followed=followed,
    profile_user=profile_user)


@app.route('/<username>/follow')
def follow_user(username):
  """ Adds the current user as follower of the given user. """
  if not g.user:
    abort(401)
  whom_id = get_user_id(username)
  if whom_id is None:
    abort(404)
  db = get_db().cursor()
  db.execute('INSERT INTO follower (who_id, whom_id) VALUES (%s, %s)',
             [session['user_id'], whom_id])
  flash('You are now following "%s"' % username)
  return redirect(url_for('user_timeline', username=username))


@app.route('/<username>/unfollow')
def unfollow_user(username):
  """ Removes the current user as follower of the given user. """
  if not g.user:
    abort(401)
  whom_id = get_user_id(username)
  if whom_id is None:
    abort(404)
  db = get_db().cursor()
  db.execute('DELETE FROM follower WHERE who_id = %s and whom_id = %s',
             [session['user_id'], whom_id])
  flash('You are no longer following "%s"' % username)
  return redirect(url_for('user_timeline', username=username))


@app.route('/add_message', methods=['POST'])
def add_message():
  """ Registers a new message for the user. """
  if 'user_id' not in session:
    abort(401)
  if request.form['text']:
    db = get_db().cursor()
    db.execute('''INSERT INTO message (author_id, text, pub_date)
                  VALUES (%s, %s, %s)''',
               (session['user_id'], request.form['text'], int(time.time())))
    flash('Your message was recorded.')
  return redirect(url_for('timeline'))


@app.route('/del_message/<int:id>')
def del_message(id):
  """ Deletes a message for the given user. """
  if 'user_id' not in session:
    abort(401)
  db = get_db().cursor()
  db.execute('''DELETE FROM message WHERE author_id=%s and message_id=%''',
             [session['user_id'], id])
  flash('Your message was deleted.')
  return redirect(url_for('timeline'))


@app.route('/login', methods=['GET', 'POST'])
def login():
  """ Logs the user in. """
  if g.user:
    return redirect(url_for('timeline'))
  error = None
  if request.method == 'POST':
    user = query_db('''SELECT * FROM user WHERE username=%s''',
                    [request.form['username']], one=True)
    if user is None:
      error = 'Invalid username'
    elif not check_password_hash(user['pw_hash'], request.form['password']):
      error = 'Invalid password'
    else:
      flash('You were logged in')
      session['user_id'] = user['user_id']
      return redirect(url_for('timeline'))
  return render_template('login.html', error=error)

@app.route('/register', methods=['GET', 'POST'])
def register():
  """ Registers the user. """
  if g.user:
    return redirect(url_for('timeline'))
  error = None
  if request.method == 'POST':
    if not request.form['username']:
      error = 'You have to enter a username'
    elif not request.form['email'] or '@' not in request.form['email']:
      error = 'You have to enter a valid email address'
    elif not request.form['password']:
      error = 'You have to enter a password'
    elif request.form['password'] != request.form['password2']:
      error = 'The two passwords do not match'
    elif get_user_id(request.form['username']) is not None:
      error = 'The username is already taken'
    else:
      db = get_db().cursor()
      db.execute('''INSERT INTO user (username, email, pw_hash)
                    VALUES (%s, %s, %s)''',
                 [request.form['username'], request.form['email'],
                  generate_password_hash(request.form['password'])])
      flash('You were successfuly registered and can login now')
      return redirect(url_for('login'))
  return render_template('register.html', error=error)


@app.route('/logout')
def logout():
  """ Logs the user out. """
  flash('You were logged out.')
  session.pop('user_id', None)
  return redirect(url_for('public_timeline'))


# Add some filters to jinja.
app.jinja_env.filters['datetimeformat'] = format_datetime
app.jinja_env.filters['gravatar'] = gravatar_url


if __name__ == '__main__':
  app.run(host="0.0.0.0")
