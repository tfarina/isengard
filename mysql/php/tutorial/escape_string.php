<?

$host = "localhost";
$user = "vrossi";
$pass = "vr46";
$db = "valentino";

function execute_query($query) {
    $r = mysql_query($query);

    if (!$r) {
        echo "Cannot execute query: $query\n";
        trigger_error(mysql_error());
    } else {
        echo "Query: $query executed\n";
    }
}

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

$query = "CREATE TABLE IF NOT EXISTS author(
        id    INT  PRIMARY KEY  AUTO_INCREMENT,
        name  VARCHAR(25))
        ENGINE=InnoDB";
execute_query($query);

$name = "O'Neill";
$name_es = mysql_real_escape_string($name);

$query = "INSERT INTO author (name) VALUES ('$name_es')";

$rs = mysql_query($query);

if (!$rs) {
    echo "Could not execute query: $query\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Query: $query executed\n";
}

mysql_close();

?>
