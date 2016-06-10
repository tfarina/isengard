<?php

require_once 'config.php';

if (isset($_POST['signup'])) {
  foreach($_POST as $key => $value) {
    $data[$key] = mysql_real_escape_string($value);
  }

  /* Before inserting, we need to do some checks, like basically if it already
     exists. */
  $nameError = "";
  $emailError = "";
  $passwordError = "";
  $valid = true;

  // Validate fullname.
  if (empty($data['fullname'])) {
    $nameError = "What's your name?";
    $valid = false;
  }

  if (empty($data['email'])) {
    $emailError = "Please enter your email.";
    $valid = false;
  } else {
    if (!filter_var($data['email'], FILTER_VALIDATE_EMAIL)) {
      $emailError = "Please enter a valid email address.";
      $valid = false;
    } else {
      $query = "SELECT email FROM user WHERE email='" . $data['email']. "' LIMIT 1;";
      $result = mysql_query($query) or die(mysql_error());
      $num_rows = mysql_num_rows($result);
      mysql_free_result($result);

      if ($num_rows > 0) {
        $emailError = "This email is already registered.";
        $valid = false;
      }
    }
  }

  if (empty($data['password']) || strlen($data['password']) < 6) {
    $passwordError = "Your password must be at least 6 characters.";
    $valid = false;
  }

  $plain_password = $data['password'];
  echo $plain_password . ' ';
  $hashed_password = PwdHash($plain_password);
  echo $hashed_password . ' ';

  $secure_password = password_hash($plain_password, PASSWORD_BCRYPT, array('cost' => 12));
  echo $secure_password;

  if ($valid) {
    $query = "INSERT INTO user (fullname, email, password) VALUES ('$data[fullname]','$data[email]','$secure_password')";
    echo $query;
    mysql_query($query) or die(mysql_error());
  }
}

include_once("header.php");
?>
<body style="font-family:'Helvetica Neue',Arial,Helvetica,sans-serif;background-color: #fafafa;">
<div class="panel panel-default panel-signin">
  <div class="panel-body">
    <form action="" method="post">
      <h3>Join us today.</h3>
      <div class="form-group <?php echo !empty($nameError) ? 'has-error' : ''; ?>">
        <input type="text" name="fullname" id="user-fullname" class="form-control"
               value="<?php echo $_POST['fullname']; ?>"
               placeholder="Full name" required autofocus/>
        <?php if (!empty($nameError)) { ?>
        <span class="help-block"><?php echo $nameError; ?></span>
        <?php } ?>
      </div>
      <div class="form-group <?php echo !empty($emailError) ? 'has-error' : ''; ?>">
        <input type="email" class="form-control" id="user-email" name="email"
               value="<?php echo $_POST['email']; ?>"
               placeholder="Email" required />
        <?php if (!empty($emailError)) { ?>
        <span class="help-block"><?php echo $emailError; ?></span>
        <?php } ?>
      </div>
      <div class="form-group <?php echo !empty($passwordError) ? 'has-error' : ''; ?>">
        <input type="password" class="form-control" id="user-password" name="password"
               placeholder="Password" required />
        <?php if (!empty($passwordError)) { ?>
        <span class="help-block"><?php echo $passwordError; ?></span>
        <?php } ?>
      </div>
      <div class="form-group">
        <button type="submit" name="signup" class="btn btn-lg btn-primary btn-block">Sign up</button>
      </div>
      <p class="text-muted text-center">
        By signing up, you agree to our <a class="legal-link"href="" target="_blank">Terms</a> and 
        <a class="legal-link" href="" target="_blank">Privacy Policy</a>.
      </p>
    </form>
  </div>
</div>
<p></p>
<div class="panel panel-default panel-signup">
  <div class="panel-body">
    <p>Have an account? <a href="login.php">Log in</a></p>
  </div>
</div>
</body>
</html>
