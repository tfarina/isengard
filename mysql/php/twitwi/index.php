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

<form method="POST" action="add.php">
  <p>Your status:</p>
  <textarea name="body" rows="5" cols="40" wrap="VIRTUAL"></textarea>
  <p><input type="submit" value="submit"/></p>
</form>

</body>
</html>
