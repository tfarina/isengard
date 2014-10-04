<?
include("distwi.php");

$username = get_post_param_trim("username");
$password = get_post_param_trim("password");
$password2 = get_post_param_trim("password2");

if (!$username || !$password || !$password2)
    go_back("Every field of the registration form is needed!");

if ($password != $password2)
    go_back("Passwords must match!");

$query = sprintf("SELECT 1 FROM user WHERE login='%s'",
    mysql_real_escape_string($username));

$result = mysql_query($query);
if (mysql_num_rows($result) != 0) {
    go_back("Sorry, the selected username is already in use.");
}

$auth_secret = get_rand();
echo $auth_secret;

// TODO(tfarina): save auth_secret in the user table. We will use it later to
// retrieve the user information in the isLoggedIn() function.
$query = sprintf("INSERT INTO `user` (login, pw) VALUES ('%s', '%s')",
    mysql_real_escape_string($username),
    mysql_real_escape_string($password));
mysql_query($query) or die(mysql_error());

mysql_close($connection);

// Expires in 1 Year.
setcookie("auth", $auth_secret, time() + 3600 * 24 * 365);

include("header.php");
?>
<h2>Welcome aboard!</h2>
Hey <?=utf8entities($username)?>, now you have an account, <a href="index.php">a good start is to write your first message!</a>.
<?
include("footer.php")
?>
