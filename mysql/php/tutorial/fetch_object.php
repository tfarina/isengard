<?

$host = "localhost";
$user = "vrossi";
$pass = "vr46";
$db = "valentino";

$r = mysql_connect($host, $user, $pass);

if (!$r) {
    echo "Could not connect to server\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Connection established\n";
}

$r2 = mysql_select_db($db);

if (!$r2) {
    echo "Cannot select database\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Database selected\n";
}

$name = "Volkswagen";

$query = sprintf("SELECT * FROM cars WHERE name = '%s'",
    mysql_real_escape_string($name));

$rs = mysql_query($query);

if (!$rs) {
    echo "Could not execute query: $query\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Query: $query executed\n";
}

while ($row = mysql_fetch_object($rs)) {
    echo $row->id;
    echo " ";
    echo $row->name;
    echo " ";
    echo $row->price;
    echo "\n";
}

mysql_close();

?>
