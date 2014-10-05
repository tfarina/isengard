<?

function add_post($user_id, $body) {
    $query = "INSERT INTO post (user_id, body, stamp) VALUES ('$user_id',
            '" . mysql_real_escape_string($body) . "', now())";

    $result = mysql_query($query);
}

?>
