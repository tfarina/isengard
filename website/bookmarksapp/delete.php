<?php

include('config.php');

mysql_query("DELETE FROM bookmarks WHERE id='" . $_GET['id'] . "'");

echo mysql_affected_rows() ? "The bookmark has been deleted successfully. <br />"
                           : "Nothing deleted. <br />";

?>

<a href='list.php'>Back To Listing</a>
