<?

include("db.php");

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

?>
