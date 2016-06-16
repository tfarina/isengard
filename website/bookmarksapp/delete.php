<?php

require_once 'config.php';

$id = -1;

if (!empty($_GET['id'])) {
  $id = $_GET['id'];
}

if ($id == -1) {
  header('Location: list.php');
}

$query = "DELETE FROM bookmarks WHERE id='" . $id . "'";
mysql_query($query) or die(mysql_error());

$message = "Bookmark has been deleted successfully.";
$_SESSION['msg-success'] = $message;
header("Location: list.php");
exit;
?>
