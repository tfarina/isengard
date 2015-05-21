<?
session_start();

include("db.php");
include("functions.php");

$id = $_GET['id'];
$do = $_GET['do'];

switch ($do) {
  case "follow":
    follow_user($_SESSION['user_id'], $id);
    $msg = "You have followed a user!";
    break;
  case "unfollow":
    unfollow_user($_SESSION['user_id'], $id);
    $msg = "You have unfollowed a user!";
    break;
}

$_SESSION['message'] = $msg;

header("Location: index.php");
?>
