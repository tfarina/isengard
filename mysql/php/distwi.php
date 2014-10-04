<?

include("db.php");

function isLoggedIn() {
    if (isset($_COOKIE['auth'])) {
        $auth_cookie = $_COOKIE['auth'];
        echo $auth_cookie;
    }
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
