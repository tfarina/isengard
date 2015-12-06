<?php

define("DB_HOST", "localhost");
define("DB_USERNAME", "justin");
define("DB_PASSWORD", "46656");

$link = mysql_connect(DB_HOST, DB_USERNAME, DB_PASSWORD);
if (!$link) {
    echo "Failed to connect to mysql server.\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Connection established\n";
}

$query = "SELECT VERSION()";

$result = mysql_query($query);
if (!$result) {
    echo "Could not execute query: $query\n";
    trigger_error(mysql_error(), E_USER_ERROR);
} else {
    echo "Query: $query executed\n";
}

$row = mysql_fetch_row($result);

echo "Version: $row[0]\n";

mysql_close();

?>
