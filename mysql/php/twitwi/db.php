<?

define("DB_SERVER", "localhost");
define("DB_USERNAME", "twin");
define("DB_PASSWORD", "catheham");
define("DB_NAME", "twitwi");

$connection = mysql_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD);
if (!$connection) {
  die('Failed to connect to the database: ' . mysql_error());
}

$db_selected = mysql_select_db(DB_NAME);
if (!$db_selected) {
  die('Failed to select the database: ' . mysql_error());
}

?>
