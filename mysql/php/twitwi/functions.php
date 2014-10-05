<?

function add_post($user_id, $body) {
    $query = "INSERT INTO post (user_id, body, stamp) VALUES ('$user_id',
            '" . mysql_real_escape_string($body) . "', now())";

    $result = mysql_query($query);
}

function show_posts($user_id) {
    $posts = array();

    $query = "SELECT body, stamp FROM post WHERE user_id = '$user_id' ORDER BY stamp DESC";
    $result = mysql_query($query);

    while ($data = mysql_fetch_object($result)) {
        $posts[] = array(
                'stamp' => $data->stamp,
                'user_id' => $user_id,
                'body' => $data->body
        );
    }

    return $posts;
}

?>
