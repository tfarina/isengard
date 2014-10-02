<?
include("distwi.php");

$username = get_post_param_trim("username");
$password = get_post_param_trim("password");
$password2 = get_post_param_trim("password2");

if (!$username || !$password || !$password2)
    go_back("Every field of the registration form is needed!");

if ($password != $password2)
    go_back("Passwords must match!");

$query = sprintf("INSERT INTO `user` (login, pw) VALUES ('%s', '%s')",
    mysql_real_escape_string($username),
    mysql_real_escape_string($password));
mysql_query($query) or die(mysql_error());

mysql_close($connection);

include("header.php");
?>
<h2>Welcome aboard!</h2>
Hey <?=utf8entities($username)?>, now you have an account, <a href="index.php">a good start is to write your first message!</a>.
<?
include("footer.php")
?>
