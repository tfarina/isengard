<?

function isLoggedIn() {
    return false;
}

function get_post_param($param) {
    global $_POST;

    if (isset($_POST[$param]))
        return $_POST[$param];
    return false;
}

function get_post_param_trim($param) {
    $val = get_post_param($param);
    if ($val === false)
         return false;
    return trim($val);
}

$db = new SQLite3('users.db');

$db->exec('CREATE TABLE IF NOT EXISTS user (
           uid INTEGER PRIMARY KEY,
           login TEXT UNIQUE,
           pw TEXT,
           email TEXT)');

$conn = mysql_connect('localhost', 'vrossi', 'vr46');

if (!$conn) {
  die('Could not connect: ' . mysql_error());
}

echo 'Connected successfully';
mysql_close($conn);

?>
