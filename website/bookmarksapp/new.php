<?php

include('config.php');

if (isset($_POST['submitted'])) {
  foreach($_POST as $key => $value) {
    $_POST[$key] = mysql_real_escape_string($value);
  }

  $sql = "INSERT INTO bookmarks (url, title) VALUES ('" . $_POST['url'] . "','" . $_POST['title'] . "')";

  mysql_query($sql) or die(mysql_error());

  $message = "The bookmark has been created successfully.";
}

?>
<?php
include_once("header.php");
?>
  <h2>Create a new bookmark</h2>
  <form action='' method='POST'>
    <div class="message">
      <?php
        if (isset($message)) {
          echo $message;
        }
      ?>
    </div>
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
