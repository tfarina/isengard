<?
include("distwi.php");

if (!isLoggedIn()) {
    header("Location: index.php");
    exit;
}

$new_auth_secret = get_rand();
$user_id = $User['id'];

$query = sprintf("UPDATE user SET cookie = '%s' WHERE uid='%s'", $new_auth_secret, $user_id);

$result = mysql_query($query);

header("Location: index.php");
?>
