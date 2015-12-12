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

?>
