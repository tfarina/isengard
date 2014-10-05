<?
session_start();

include("db.php");
include("functions.php");

$user_id = $_SESSION['user_id'];
$body = substr($_POST['body'], 0, 140);

add_post($user_id, $body);

$_SESSION['message'] = "Your post has been added!";

header("Location: index.php");
?>
