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

function isUserLoggedIn() {
  return isset($_SESSION) && isset($_SESSION['userid']);
}

function logout() {
  session_unset();
  session_destroy();
  session_write_close();
}

define('SALT_LENGTH', 9);

function PwdHash($pwd, $salt = null) {
  if ($salt === null) {
    $salt = substr(md5(uniqid(rand(), true)), 0, SALT_LENGTH);
  } else {
    $salt = substr($salt, 0, SALT_LENGTH);
  }
  return $salt . sha1($pwd . $salt);
}

function email_exists($email) {
  $query = "SELECT email FROM user WHERE email='" . $email . "' LIMIT 1;";
  $result = mysql_query($query) or die(mysql_error());

  $num_rows = mysql_num_rows($result);
  mysql_free_result($result);

  return ($num_rows > 0) ? true : false; 
}

?>
