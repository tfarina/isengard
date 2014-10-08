<?

$host = "localhost";
$user = "vrossi";
$pass = "vr46";

$r = mysql_connect($host, $user, $pass);

if (!$r) {
    echo "Could not connect to server\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Connection established\n";
}

$query = "SELECT VERSION()";

$rs = mysql_query($query);

if (!$rs) {
    echo "Could not execute query: $query\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Query: $query executed\n";
}

$row = mysql_fetch_row($rs);

echo "Version: $row[0]\n";

mysql_close();

?>
