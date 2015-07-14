'''
   Config options for the stripe_payment app.
   Author: Muneeb Ali | http://muneeb.org
   License: MIT (see LICENSE)
'''

import os

# Put the following lines in your ~/.bashrc.
# export PUBLISHABLE_KEY=<your_stripe_publishable_key>
# export SECRET_KEY=<your_stripe_secrete_key>
stripe_keys = {
  'secret_key': os.environ['SECRET_KEY'],
  'publishable_key': os.environ['PUBLISHABLE_KEY']
}

DEFAULT_CHARGE_AMOUNT = 525 # in cents.
DEFAULT_CHARGE_DESCRIPTION = "Flask Charge"

DB_NAME = "stripe_customers"
