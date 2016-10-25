<?

function add_post($user_id, $body) {
    $query = "INSERT INTO post (user_id, body, stamp) VALUES ('$user_id',
            '" . mysql_real_escape_string($body) . "', now())";

    $result = mysql_query($query);
}

function get_username($user_id) {
    $query = "SELECT login FROM user WHERE uid='$user_id'";
    $result = mysql_query($query);

    $row = mysql_fetch_assoc($result);
    return $row['login'];
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

function show_users() {
    $users = array();
    $query = "SELECT uid, login FROM user ORDER BY login";
    $result = mysql_query($query);

    while ($data = mysql_fetch_object($result)) {
        $users[$data->uid] = $data->login;
    }

    return $users;
}

// Who is this user following?
function following($user_id) {
    $users = array();

    $query = "SELECT DISTINCT user_id FROM following WHERE follower_id = '$user_id'";
    $result = mysql_query($query);

    while ($data = mysql_fetch_object($result)) {
        array_push($users, $data->user_id);
    }

    return $users;
}

function check_count($first, $second) {
  $query = "SELECT COUNT(*) FROM following WHERE user_id='$second' and follower_id='$first'";
  $result = mysql_query($query);

  $row = mysql_fetch_row($result);
  return $row[0];
}

function follow_user($me, $them) {
  $count = check_count($me, $them);

  if ($count == 0) {
    $query = "INSERT INTO following (user_id, follower_id) VALUES ($them, $me)";
    $result = mysql_query($query);
  }
}

function unfollow_user($me, $them) {
  $count = check_count($me, $them);

  if ($count != 0) {
    $query = "DELETE FROM following WHERE user_id='$them' and follower_id='$me' LIMIT 1";
    $result = mysql_query($query);
  }
}

// http://gilbert.pellegrom.me/php-relative-time-function/
function relative_time($date, $postfix = ' ago', $fallback = 'F Y') {
    $diff = time() - strtotime($date);
    if ($diff < 60)
        return $diff . ' second'. ($diff != 1 ? 's' : '') . $postfix;
    $diff = round($diff/60);
    if ($diff < 60)
        return $diff . ' minute'. ($diff != 1 ? 's' : '') . $postfix;
    $diff = round($diff/60);
    if ($diff < 24)
        return $diff . ' hour'. ($diff != 1 ? 's' : '') . $postfix;
    $diff = round($diff/24);
    if ($diff < 7)
        return $diff . ' day'. ($diff != 1 ? 's' : '') . $postfix;
    $diff = round($diff/7);
    if ($diff < 4)
        return $diff . ' week'. ($diff != 1 ? 's' : '') . $postfix;
    $diff = round($diff/4);
    if ($diff < 12)
        return $diff . ' month'. ($diff != 1 ? 's' : '') . $postfix;
    return date($fallback, strtotime($date));
}

?>
