'''
   A simple Flask app for making Stripe payments.
   Author: Muneeb Ali | http://muneeb.org
   Modified from examples at http://stripe.com/docs
   License: MIT (see LICENSE)
'''

import os
from flask import Flask, render_template, request
import stripe

CHARGE_AMOUNT = 525 # in cents.
CHARGE_DESCRIPTION = "Flask Charge"

# Put the following lines in your ~/.bashrc.
# export SECRET_KEY=<your_stripe_secrete_key>
# export PUBLISHABLE_KEY=<your_stripe_publishable_key>
stripe_keys = {
  'secret_key': os.environ['SECRET_KEY'],
  'publishable_key': os.environ['PUBLISHABLE_KEY']
}

stripe.api_key = stripe_keys['secret_key']

app = Flask(__name__)


def get_amount_usd(amount=CHARGE_AMOUNT):
  return float(amount) / 100


def stripe_charge(token,
                  charge_amount=CHARGE_AMOUNT,
                  charge_description=CHARGE_DESCRIPTION):
  try:
    charge = stripe.Charge.create(
        amount=charge_amount,
        currency='usd',
        source=token,
        description=charge_description)
  except stripe.error.CardError,  e:
    return False, e

  return True, "successfully charged"

@app.route('/')
def index():
  return render_template('index.html',
                         key=stripe_keys['publishable_key'],
                         amount_usd=get_amount_usd())


@app.route('/charge', methods=['POST'])
def charge():
  customer_email = request.form['email']

  customer = stripe.Customer.create(
    email=customer_email,
    card=request.form['stripeToken'])

  # Get the credit card details submitted by the form
  token = request.form['stripeToken']
  charge_valid, charge_msg = stripe_charge(token)
  return render_template('charge.html',
                         amount_usd=get_amount_usd(),
                         customer_email=customer_email,
                         charge_valid=charge_valid,
                         charge_msg=charge_msg)


if __name__ == '__main__':
  app.run(debug=True)
