<?php

// https://www.developphp.com/video/PHP/Pagination-MySQLi-Google-Style-Paged-Results-Tutorial
// http://www.phpfreaks.com/tutorial/basic-pagination

// mysql> CREATE USER 'testuser'@'localhost' IDENTIFIED BY 'testpass';
// mysql> GRANT ALL PRIVILEGES ON test.* TO 'testuser'@'localhost';
// mysql> FLUSH PRIVILEGES;

$CONF['db_host'] = "localhost";
$CONF['db_username'] = "testuser";
$CONF['db_password'] = "testpass";
$CONF['db_name'] = "test";

$link = @mysql_connect($CONF['db_host'], $CONF['db_username'], $CONF['db_password']);
if (!$link) {
  die('Could not connect to mysql: ' . mysql_error());
}

if (!mysql_select_db($CONF['db_name'], $link)) {
  die ('Could not select database ' . $CONF['db_name'] . ': ' . mysql_error());
}

// Find out how many rows are in the table.
$t_query = "SELECT COUNT(id) AS articles_count FROM articles";
$t_result = mysql_query($t_query) or die (mysql_error());
$t_row = mysql_fetch_assoc($t_result);
$t_articles_count = $t_row['articles_count'];

//echo $t_articles_count;

// The number of entries to show per page.
$perpage = 10;

//echo "\n";

// Find out how many pages we will have.
$lastpage = ceil($t_articles_count / $perpage);
    
//echo $lastpage;

if ($lastpage < 1) {
  $lastpage = 1;
} 

// The page currently returned.
$pagenum = 1;

// Get the current page.
if (isset($_GET['page']) && is_numeric($_GET['page'])) {
  $pagenum = preg_replace('#[^0-9]#', '', $_GET['page']);
}

//echo $pagenum;

// Make sure $pagenum isn't before the first page.
if ($pagenum < 1) {
  $pagenum = 1;
}

// Make sure $pagenum isn't past the last page.
if ($pagenum > $lastpage) {
  $pagenum = $lastpage;
}

$offset = ($pagenum - 1) * $perpage;

$t_query = "SELECT * FROM articles LIMIT $offset, $perpage";
$t_result = mysql_query($t_query) or die (mysql_error());

$t_articles = array();
while ($t_row = mysql_fetch_assoc($t_result)) {
  $t_articles[] = $t_row;
}

$start = $offset + 1;
$to = (($perpage * $pagenum) > $t_articles_count) ? $t_articles_count : ($perpage * $pagenum);

$pagination = '';

// If there is more than one page worth of results then we have to show
// the pagination system, otherwise it is not worth to show it.
if ($lastpage > 1) {
  $pagination .= '<div class="pager">';
  $pagination .= "\n";

  if ($pagenum > 1) {
    $previous = $pagenum - 1;
   
    $pagination .= '<a href="'.$_SERVER['PHP_SELF'].'?page=1"><span class="page-numbers first">First</span></a> &nbsp; ';
    $pagination .= "\n";
    $pagination .= '<a href="'.$_SERVER['PHP_SELF'].'?page='.$previous.'" rel="prev"><span class="page-numbers prev">Previous</span></a> &nbsp; &nbsp; ';
    $pagination .= "\n";

    for ($i = $pagenum - 4; $i < $pagenum; $i++) {
      if ($i > 0) {
        $pagination .= '<a href="' . $_SERVER['PHP_SELF'] . '?page=' . $i . '"><span class="page-numbers">' . $i . '</span></a> &nbsp; ';
        $pagination .= "\n";
      }
    }
  }

  $pagination .= '<span class="page-numbers current">'. $pagenum .'</span> &nbsp; ';
  $pagination .= "\n";

  for ($i = $pagenum + 1; $i <= $lastpage; $i++) {
    $pagination .= '<a href="' . $_SERVER['PHP_SELF'] . '?page=' . $i . '"><span class="page-numbers">' . $i . '</span></a> &nbsp; ';

    if ($i >= $pagenum + 4) {
      break;
    }
  }

  if ($pagenum != $lastpage) {
    $next = $pagenum + 1;

    $pagination .= '&nbsp; <a href="'.$_SERVER['PHP_SELF'].'?page='.$next.'" rel="next"><span class="page-numbers next">Next</span></a> &nbsp; ';
    $pagination .= '<a href="'.$_SERVER['PHP_SELF'].'?page='.$lastpage.'"><span class="page-numbers last">Last</span></a>';
  }

  $pagination .= '</div>';
}

?>
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Pagination</title>
</head>
<body>

<?php 
echo "<p>Showing " . $start . " - " . $to . " of " . $t_articles_count . " results</p>";

foreach ($t_articles as $t_article) {
  echo "<p>" . $t_article['title'] . "</p>\n";
}

echo $pagination;
?>

</body>
</html>
