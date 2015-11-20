<?

$host = "localhost";
$user = "justin";
$pass = "46656";
$db = "php_tutorial";

$r = mysql_connect($host, $user, $pass);

if (!$r) {
    echo "Could not connect to server\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Connection established\n";
}

if (!mysql_select_db($db)) {
    echo "Cannot select database\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Database selected\n";
}

$query = "SELECT * FROM cars LIMIT 5";

$rs = mysql_query($query);

if (!$rs) {
    echo "Could not execute query: $query";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Query: $query executed\n";
}

while ($row = mysql_fetch_assoc($rs)) {
    echo $row['id'] . " " . $row['name'] . " " . $row['price'] . "\n";
}

mysql_close();

?>
