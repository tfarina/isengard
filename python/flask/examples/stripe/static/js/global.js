// Author: Muneeb Ali. http://muneeb.org
// Modified from examples at http://stripe.com/docs

function stripeResponseHandler(status, response) {
  var $form = $('#payment-form');

  if (response.error) {
    // Show the error on the form.
    $form.find('.payment-errors').text(response.error.message);
    $form.find('button').prop('disabled', false);
  } else {
    // Insert the token into the form so it gets submitted to the server.
     $("input[name=stripeToken]").val(response.id);
    // And submit.
    $form.get(0).submit();
  }
};

jQuery(function($) {
  $('#payment-form').submit(function(event) {
    var $form = $(this);

    // Disable the submit button to prevent repeated clicks.
    $form.find('button').prop('disabled', true);

    Stripe.card.createToken($form, stripeResponseHandler);

    // Prevent the form from submitting with the default action.
    return false;
  });
});
