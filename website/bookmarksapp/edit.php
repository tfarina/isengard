<?php
include('config.php');

function is_num($string) {
  if (preg_match("/^[0-9]+$/i", $string)) {
    return true;
  }
  return false;
}

$id = "-1";
if (isset($_GET['id']) && is_num($_GET['id'])) {
  $id = $_GET['id'];
}

if ($id == "-1") {
  include_once("header.php");
  echo "     <div class=\"error\">Error: record id not found</div>\n";
  include_once("footer.php");
  exit;
}

$query = "SELECT COUNT(id) AS exist FROM bookmarks WHERE id='" . $id . "'";
$result = mysql_query($query) or die(mysql_error());
list($total) = mysql_fetch_row($result);
if ($total == 0) {
  include_once("header.php");
  echo "     <div class=\"error\">Error: There is no bookmark with this ID.</div>\n";
  include_once("footer.php");
  exit;
}

if (isset($_POST['commit'])) {
  foreach ($_POST as $key => $value) {
    $_POST[$key] = mysql_real_escape_string($value);
  }

  $query = "UPDATE bookmarks SET url='" . $_POST['url'] . "', title='" . $_POST['title'] . "' WHERE id='" . $id . "'";
  mysql_query($query) or die(mysql_error());

  if (mysql_affected_rows()) {
    $message = "Bookmark has been updated successfully.\n";
  } else {
    $message = "Nothing changed.\n";
  }
}

$query = "SELECT * FROM bookmarks WHERE id='" . $id . "'";
$result = mysql_query($query);
$row = mysql_fetch_assoc($result);
mysql_free_result($result);
?>
<?php
include_once("header.php");
?>
  <?php if (isset($message)) { ?>
  <div class="message">
    <?php echo $message; ?>
  </div>
  <?php } ?>
  <h3>Edit bookmark</h3>
  <form action="" method="POST">
  <table>
    <tbody>
    <tr>
      <td align="right">Title:</td>
      <td><input type="text" name="title" value="<?php echo stripslashes($row['title']) ?>"/></td>
    </tr>
    <tr>
      <td align="right">URL:</td>
      <td><input type="text" name="url" value="<?php echo stripslashes($row['url']) ?>"/></td>
    </tr>
    <tr>
      <td></td>
      <td style="float: right">
        <button type="submit" name="commit" class="pure-button pure-button-primary">Save</button>
      </td>
    </tr>
    </tbody>
  </table>
</form>
<?php
include_once("footer.php");
?>
