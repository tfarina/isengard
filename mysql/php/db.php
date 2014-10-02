<?

$hostname = 'localhost';
$username = 'vrossi';
$password = 'vr46';

$connection = mysql_connect($hostname, $username, $password);
if (!$connection) {
  die('Failed to connect to the database: ' . mysql_error());
}

$db_selected = mysql_select_db('valentino');
if (!$db_selected) {
  die('Failed to select the database: ' . mysql_error());
}

//mysql_close($connection);

?>
