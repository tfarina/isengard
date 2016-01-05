<?php

include('config.php');

$query = "DELETE FROM bookmarks WHERE id='" . $_GET['id'] . "'";
mysql_query($query) or die(mysql_error());

if (mysql_affected_rows()) {
  $message = "The bookmark has been deleted successfully.\n";
} else {
  $message = "Nothing deleted.\n";
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
<?php
include_once("footer.php");
?>
