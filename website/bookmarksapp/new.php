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
  <h2>Create a new bookmark</h2>
  <form action='' method='POST'>
    <table>
      <tr>
        <td>Title</td>
        <td><input type="text" name='title'/></td>
      </tr>
      <tr>
        <td>URL</td>
        <td><input type="text" name='url'/></td>
      </tr>
      <tr>
        <td>&nbsp;</td>
        <td>
          <input type='submit' value='Save'/>
          <input type='hidden' value='1' name='submitted'/>
          <a href="list.php">Back to bookmarks list</a>
        </td>
      </tr>
    </table>
  </form>
<?php
include_once("footer.php");
?>
