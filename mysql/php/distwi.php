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

function utf8entities($s) {
    return htmlentities($s,ENT_COMPAT,'UTF-8');
}


$db = new SQLite3('users.db');

$db->exec('CREATE TABLE IF NOT EXISTS user (
           uid INTEGER PRIMARY KEY,
           login TEXT UNIQUE,
           pw TEXT,
           email TEXT)');

$hostname = 'localhost';
$username = 'vrossi';
$password = 'vr46';

$connection = mysql_connect($hostname, $username, $password);
if (!$connection) {
  die('Failed to connect to the database: ' . mysql_error());
}

echo "Connected successfully\n";

$db_selected = mysql_select_db('valentino');
if (!$db_selected) {
  die('Database selection failed ' . mysql_error());
}

//mysql_close($connection);

?>
