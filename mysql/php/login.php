<?
include("distwi.php");

$username = get_post_param_trim("username");
$password = get_post_param_trim("password");

if (!$username || !$password)
    go_back("You need to enter both username and password to login.");

$query = sprintf("SELECT uid, cookie FROM user WHERE login='%s' AND length(pw) > 0 AND pw='%s'",
    mysql_real_escape_string($username),
    mysql_real_escape_string($password));

$result = mysql_query($query);
if (mysql_num_rows($result) == 0) {
    go_back("Wrong username or password.");
}

$row = mysql_fetch_assoc($result);
$auth_secret = $row['cookie'];

// TODO(tfarina): Check if auth_secret is NULL, it might be!
setcookie("auth", $auth_secret, time() + 3600 * 24 * 365);

header("Location: index.php");
?>
