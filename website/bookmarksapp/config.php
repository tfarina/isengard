<?php

define("DB_SERVER", "localhost");
define("DB_USERNAME", "seafowl");
define("DB_PASSWORD", "2Mnc7nXm");
define("DB_NAME", "bkms");

$link = mysql_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD);
if (!$link) {
  die('Not connected : ' . mysql_error());
}

if (!mysql_select_db(DB_NAME)) {
  die ('Could not select' . DB_NAME . ' : ' . mysql_error());
}

?>
