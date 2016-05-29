<?php

include('config.php');

$host = "localhost";
$user = "";
$pass = "";
$db = "";

$r = mysql_connect($host, $user, $pass);
if (!$r) {
  echo "Could not connect to server\n";
  trigger_error(mysql_error(), E_USER_ERROR);
}

if (!mysql_select_db($db)) {
  echo "Cannot select database\n";
  trigger_error(mysql_error(), E_USER_ERROR);
}

// mysql_query("INSERT INTO bookmarks (hash, url, title) VALUES ('".md5($_GET['url'])."', '".$_GET['url']."', '".$_GET['title']."')");
?>

<!doctype html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Bookmark manager</title>
<link rel="stylesheet" type="text/css" href="styles.css" />
</head>
<body>
  <div id="main">
    <ul>
      <?php
        $bookmarks = mysql_query("SELECT * FROM bookmarks ORDER BY id DESC");
        while ($row = mysql_fetch_assoc($bookmarks)) {
          echo '<li><a href="'.$row['url'].'">'.$row['title'].'</a></li>';
        }
      ?>
    </ul>
  </div>
</body>
</html>
