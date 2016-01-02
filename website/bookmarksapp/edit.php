<?php
include('config.php');

$id = isset($_GET['id']) ? $_GET['id'] : die('ERROR: record id not found');

if (isset($_POST['submitted'])) {
  foreach ($_POST as $key => $value) {
    $_POST[$key] = mysql_real_escape_string($value);
  }

  $query = "UPDATE bookmarks SET url='" . $_POST['url'] . "', title='" . $_POST['title'] . "' WHERE id='" . $id . "'";
  mysql_query($query) or die(mysql_error());

  if (mysql_affected_rows()) {
    $message = "The bookmark has been updated successfully.\n";
  } else {
    $message = "Nothing changed.\n";
  }
}

$query = "SELECT * FROM bookmarks WHERE id='" . $id . "'";
$result = mysql_query($query);
$row = mysql_fetch_assoc($result);
?>
<?php
include_once("header.php");
?>
  <?php if (isset($message)) { ?>
  <div class="message">
    <?php echo $message; ?>
  </div>
  <?php } ?>
  <h2>Edit bookmark</h2>
  <form action='' method='POST'>
  <table>
    <tr>
      <td>Title</td>
      <td><input type="text" name='title' value="<?php echo stripslashes($row['title']) ?>"/></td>
    </tr>
    <tr>
      <td>URL</td>
      <td><input type="text" name='url' value="<?php echo stripslashes($row['url']) ?>"/></td>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td>
        <input type='submit' value='Save Changes'/>
        <input type='hidden' value='1' name='submitted' />
      </td>
    </tr>
  </table>
</form>
  <p>
    <a href="list.php">Back to bookmarks list</a>
  </p>
<?php
include_once("footer.php");
?>
