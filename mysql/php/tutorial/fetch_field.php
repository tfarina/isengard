<?

//
// Column Headers:
//
// How to print column headers with the data from the database table.
//

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

$query = "SELECT * FROM cars LIMIT 8";

$rs = mysql_query($query);

if (!$rs) {
    echo "Could not execute query: $query";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Query: $query executed\n";
}

$id_column = mysql_fetch_field($rs, 0);
$name_column = mysql_fetch_field($rs, 1);
$price_column = mysql_fetch_field($rs, 2);

printf("%3s %-11s %8s\n", $id_column->name, $name_column->name, $price_column->name);

while ($row = mysql_fetch_row($rs)) {
    printf("%3s %-11s %8s\n", $row[0], $row[1], $row[2]);
}

mysql_close();

?>
