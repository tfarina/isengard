<?php
include('config.php');

$id = isset($_GET['id']) ? $_GET['id'] : die('ERROR: record id not found');

if (isset($_POST['submitted'])) {
  foreach ($_POST as $key => $value) {
    $_POST[$key] = mysql_real_escape_string($value);
  }

  $sql = "UPDATE bookmarks SET url='" . $_POST['url'] . "', title='" . $_POST['title'] . "' WHERE id='" . $id . "'";
  mysql_query($sql) or die(mysql_error());

  echo mysql_affected_rows() ? "Bookmark edited. <br />"
                             : "Nothing changed. <br />";
  echo "<a href='list.php'>Back To Listing</a>";
}

$result = mysql_query("SELECT * FROM bookmarks WHERE id='" . $id . "'");
$row = mysql_fetch_assoc($result);
$title = $row['title'];
$url = $row['url'];
?>
<?php
include_once("header.php");
?>
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
        <a href='list.php'>Back to bookmarks list</a>
      </td>
    </tr>
  </table>
</form>
<?php
include_once("footer.php");
?>
