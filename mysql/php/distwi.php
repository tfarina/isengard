<?

include("db.php");

function isLoggedIn() {
    if (isset($_COOKIE['auth'])) {
        $auth_cookie = $_COOKIE['auth'];
        //echo $auth_cookie;
        $query = sprintf("SELECT uid, login, email FROM user WHERE cookie='%s'", $auth_cookie);
        $result = mysql_query($query);
        if (mysql_num_rows($result) == 0)
            return false;
        $row = mysql_fetch_row($result);
        loadUserInfo($row);
        return true;
    }
    return false;
}

function loadUserInfo($row) {
    global $User;

    $User['id'] = $row[0];
    $User['name'] = $row[1];
    $User['email'] = $row[2];
    //echo "Your id is: " . $row[0];
    //echo "Your login is: " . $row[1];
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

function go_back($msg) {
    include("header.php");
    echo('<div id ="error">'.utf8entities($msg).'<br>');
    echo('<a href="javascript:history.back()">Please return back and try again</a></div>');
    include("footer.php");
    exit;
}

function get_rand() {
    $fd = fopen("/dev/urandom", "r");
    $data = fread($fd, 16);
    fclose($fd);
    return md5($data);
}

?>
