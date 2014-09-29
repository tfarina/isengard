<?
include("distwi.php");

$username = get_post_param_trim("username");
$password = get_post_param_trim("password");

$query = <<<EOD
  INSERT INTO user (login, pw) VALUES ( '$username', '$password' )
EOD;

//$db->exec($query) or die("Unable to add user $username");

// $result = $db->query('SELECT * FROM user') or die('Query failed');
// while ($row = $result->fetchArray())
//         {
//   echo "Username: {$row['login']}\nPassword: {$row['pw']}\n";
// }

$query = sprintf("INSERT INTO `user` (login, pw) VALUES ('%s', '%s')",
    mysql_real_escape_string($username),
    mysql_real_escape_string($password));
mysql_query($query) or die(mysql_error());

echo "User added!";

mysql_close($connection);

include("header.php");
?>
<h2>Welcome aboard!</h2>
Hey <?=utf8entities($username)?>, now you have an account, <a href="index.php">a good start is to write your first message!</a>.
<?
include("footer.php")
?>
