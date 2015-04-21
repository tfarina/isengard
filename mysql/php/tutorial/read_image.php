<?php

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

$r2 = mysql_select_db($db);

if (!$r2) {
    echo "Cannot select database\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Database selected\n";
}

$query = "SELECT data FROM images WHERE id=1";

$rs = mysql_query($query);

if (!$rs) {
    echo "Could not execute query: $query";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Query: $query executed\n";
}

$row = mysql_fetch_row($rs);

$file = "ducati_2.png";

$img = fopen($file, "wb");

if (!$img) {
    echo "Cannot open file for writing\n";
    trigger_error("Cannot open file for writing\n", E_USER_ERROR);
}

$r3 = fwrite($img, $row[0]);

if (!$r3) {
    echo "Cannot write image to file\n";
    trigger_error("Cannot write image to file\n", E_USER_ERROR);
}

fclose($img);

mysql_close();

?>
