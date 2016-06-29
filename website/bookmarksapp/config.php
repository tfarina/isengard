<?php

define("DB_HOST", "localhost");
define("DB_USERNAME", "seafowl");
define("DB_PASSWORD", "2Mnc7nXm");
define("DB_NAME", "bkms");

$link = mysql_connect(DB_HOST, DB_USERNAME, DB_PASSWORD);
if (!$link) {
  die('Could not connect to mysql: ' . mysql_error());
}

if (!mysql_select_db(DB_NAME, $link)) {
  die ('Could not select database ' . DB_NAME . ': ' . mysql_error());
}

session_start();

function is_user_logged_in() {
  return isset($_SESSION) && isset($_SESSION['userid']);
}

function logout() {
  session_unset();
  session_destroy();
  session_write_close();
}

function is_valid_email_address($email) {
  return filter_var($email, FILTER_VALIDATE_EMAIL);
}

function email_exists($email) {
  $query = "SELECT email FROM user WHERE email='" . $email . "' LIMIT 1;";
  $result = mysql_query($query) or die(mysql_error());

  $num_rows = mysql_num_rows($result);
  mysql_free_result($result);

  return ($num_rows > 0) ? true : false; 
}

function is_num($string) {
  if (preg_match("/^[0-9]+$/i", $string)) {
    return true;
  }
  return false;
}

function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
}

?>
