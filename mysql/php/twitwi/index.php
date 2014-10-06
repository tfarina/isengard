<?

session_start();

include("db.php");
include("functions.php");

$_SESSION['user_id'] = 1;

?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta http-equiv="content-type" content="text/html"; charset=utf-8 />
  <title>MicroBlogging App</title>
</head>
<body>
<?
if (isset($_SESSION['message'])) {
    echo "<b>". $_SESSION['message'] ."</b>";
    unset($_SESSION['message']);
}
?>

<p><a href="users.php">see list of users</a></p>
<form method="POST" action="add.php">
  <p>Your status:</p>
  <textarea name="body" rows="5" cols="40" wrap="VIRTUAL"></textarea>
  <p><input type="submit" value="submit"/></p>
</form>

<?
$posts = show_posts($_SESSION['user_id']);

if (count($posts)) {
?>
<table border="1" cellspacing="0" cellpading="5" width="500">
<?
    foreach ($posts as $key => $list) {
        echo "<tr valing='top'>\n";
        echo "<td>". $list['user_id'] ."</td>\n";
        echo "<td>". $list['body'] ."<br/>\n";
        echo "<small>". $list['stamp'] ."</small></td>\n";
        echo "</tr>\n";
    }
?>
</table>
<?
} else {
?>
<p><b>You haven't posted anything yet!</b></p>
<?
}
?>
</body>
</html>
