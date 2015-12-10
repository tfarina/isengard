<?php

include('config.php');

$result = mysql_query("SELECT * FROM bookmarks ORDER BY dt DESC") or trigger_error(mysql_error());

while ($row = mysql_fetch_assoc($result)) {
  $bookmarks[] = $row;
}

include_once("header.php");

echo "<h1>Read bookmark records</h1>";
echo "<p><a href='new.php'>Create a new bookmark</a></p>\n";

echo "<table>\n";
echo "<thead>\n";
echo "<tr>\n";
echo "<th><b>URL</b></th>\n";
echo "<th><b>Title</b></th>\n";
echo "<th><b>Date Created</b></th>\n";
echo "<th>&nbsp;</th>\n";
echo "<th>&nbsp;</th>\n";
echo "</tr>\n";
echo "</thead>\n";
echo "<tbody>\n";

foreach ($bookmarks as $bookmark) {
  echo "<tr>\n";
  echo "<td valign='top'><a href=". $bookmark['url'] .">" . $bookmark['url'] ."</a></td>\n";
  echo "<td valign='top'>" . $bookmark['title'] . "</td>\n";
  echo "<td valign='top'>" . $bookmark['dt'] . "</td>\n";
  echo "<td valign='top'><a href=edit.php?id={$bookmark['id']}>Edit</a></td><td><a href=delete.php?id={$bookmark['id']}>Delete</a></td>\n";
  echo "</tr>\n";
}

echo "</tbody>\n";
echo "</table>\n";

include_once("footer.php");

?>
