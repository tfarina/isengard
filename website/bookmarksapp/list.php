<?php

include('config.php');

echo "<p><a href=new.php>New Bookmark</a></p>\n";

echo "<table border=1>\n";
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

$result = mysql_query("SELECT * FROM bookmarks ORDER BY dt DESC") or trigger_error(mysql_error());

while ($row = mysql_fetch_assoc($result)) {
  echo "<tr>\n";
  echo "<td valign='top'><a href=". $row['url'] .">" . $row['url'] ."</a></td>\n";
  echo "<td valign='top'>" . $row['title'] . "</td>\n";
  echo "<td valign='top'>" . $row['dt'] . "</td>\n";
  echo "<td valign='top'><a href=edit.php?id={$row['id']}>Edit</a></td><td><a href=delete.php?id={$row['id']}>Delete</a></td>\n";
  echo "</tr>\n";
}

echo "</tbody>\n";
echo "</table>\n";

?>
