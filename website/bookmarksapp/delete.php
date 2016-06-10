<?php

require_once 'config.php';

$query = "DELETE FROM bookmarks WHERE id='" . $_GET['id'] . "'";
mysql_query($query) or die(mysql_error());

header("Location: list.php");
?>
