<?php

include('config.php');

if (isset($_POST['submit'])) {
  foreach($_POST as $key => $value) {
    $data[$key] = mysql_real_escape_string($value);
  }

  $query = "INSERT INTO bookmarks (url, title) VALUES ("
          . "'" . $data['url'] . "',"
          . "'" . $data['title'] . "')";
  mysql_query($query) or die(mysql_error());

  $message = "Bookmark has been created successfully.";
}

?>
<?php
include_once("header.php");
?>
  <?php if (isset($message)) { ?>
  <div class="message">
    <?php echo $message; ?>
  </div>
  <?php } ?>
  <h3>Create New Bookmark</h3>
  <form name="newBookmark" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
    <table>
      <tbody>
      <tr>
        <td align="right">Title:</td>
        <td><input type="text" name="title"/></td>
      </tr>
      <tr>
        <td align="right">URL:</td>
        <td><input type="text" name="url"/></td>
      </tr>
      <tr>
        <td></td>
        <td style="float: right;">
          <button type="submit" name="submit">Create</button>
        </td>
      </tr>
      </tbody>
    </table>
  </form>
<?php
include_once("footer.php");
?>
