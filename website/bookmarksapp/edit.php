<?php
include('config.php');
if (isset($_GET['id'])) {
  if (isset($_POST['submitted'])) {
    foreach ($_POST as $key => $value) {
      $_POST[$key] = mysql_real_escape_string($value);
    }

    $sql = "UPDATE bookmarks SET url='" . $_POST['url'] . "', title='" . $_POST['title'] . "' WHERE id='" . $_GET['id'] . "'";
    mysql_query($sql) or die(mysql_error());

    echo mysql_affected_rows() ? "Bookmark edited. <br />"
                               : "Nothing changed. <br />";
    echo "<a href='list.php'>Back To Listing</a>";
  }

  $row = mysql_fetch_array(mysql_query("SELECT * FROM bookmarks WHERE id='" . $_GET['id'] . "'"));
?>

<form action='' method='POST'>
  <p><b>Title:</b><br /><input type="text" name='title' value="<?php echo stripslashes($row['title']) ?>"/>
  <p><b>URL:</b><br /><input type="text" name='url' value="<?php echo stripslashes($row['url']) ?>"/>
  <p><input type='submit' value='Edit Bookmark' /><input type='hidden' value='1' name='submitted' />
</form>

<?php } ?>
