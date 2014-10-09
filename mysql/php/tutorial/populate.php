<?

$host = "localhost";
$user = "justin";
$pass = "46656";
$db = "php_tutorial";

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

$query = "DROP TABLE IF EXISTS cars";
execute_query($query);

$query = "CREATE TABLE cars(
        id    INT  PRIMARY KEY,
        name  TEXT,
        price INT)
        ENGINE=InnoDB";
execute_query($query);

$query = "INSERT INTO cars VALUES(1,'Audi',52642)";
execute_query($query);

$query = "INSERT INTO cars VALUES(2,'Mercedes',57127)";
execute_query($query);

$query = "INSERT INTO cars VALUES(3,'Skoda',9000)";
execute_query($query);

$query = "INSERT INTO cars VALUES(4,'Volvo',29000)";
execute_query($query);

$query = "INSERT INTO cars VALUES(5,'Bentley',350000)";
execute_query($query);

$query = "INSERT INTO cars VALUES(6,'Citroen',21000)";
execute_query($query);

$query = "INSERT INTO cars VALUES(7,'Hummer',41400)";
execute_query($query);

$query = "INSERT INTO cars VALUES(8,'Volkswagen',21600)";
execute_query($query);

mysql_close();

?>
