<?php

include('config.php');

if (isset($_POST['submitted'])) {
  foreach($_POST as $key => $value) {
    $_POST[$key] = mysql_real_escape_string($value);
  }

  $sql = "INSERT INTO bookmarks (url, title) VALUES ('" . $_POST['url'] . "','" . $_POST['title'] . "')";

  mysql_query($sql) or die(mysql_error());

  echo "Bookmark added successfully!<br />";
  echo "<a href='list.php'>Back To Listing</a>";
}

?>
<?php
include_once("header.php");
?>
  <form action='' method='POST'>
    <p><b>Title:</b><br /><input type="text" name='title'/>
    <p><b>URL:</b><br /><input type="text" name='url'/>
    <p><input type='submit' value='Add Bookmark'/>
       <input type='hidden' value='1' name='submitted'/>
  </form>
<?php
include_once("footer.php");
?>
