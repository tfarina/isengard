<?php

$username = $_POST['username'];
$password1 = $_POST['password1'];
$password2 = $_POST['password2'];
$email = $_POST['email'];

if ($password1 != $password2)
  header('Location: registration.html');

if (strlen($username) > 30)
  header('Location: registration.html');

$hash = hash('sha256', $password1);

function createSalt() {
  $text = md5(uniqid(rand(), true));
  return substr($text, 0, 3);
}

$salt = createSalt();
$password = hash('sha256', $salt . $hash);

$conn = mysql_connect('localhost', 'root', '');
mysql_select_db('login', $conn);

$username = mysql_real_escape_string($username);

$query = "INSERT INTO member(username, password, email, salt)
          VALUES ('$username', '$password', '$email', '$salt');"

mysql_query($query);

mysql_close();

header('Location: login.php');

?>
