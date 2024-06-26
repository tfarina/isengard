<?php

require_once 'config.php';

$id = "-1";
if (isset($_GET['id']) && is_num($_GET['id'])) {
  $id = $_GET['id'];
}

if ($id == "-1") {
  include_once("header.php");
  echo "     <div class=\"error\">Error: record id not found</div>\n";
  include_once("footer.php");
  exit;
}

$query = "SELECT COUNT(id) AS exist FROM bookmarks WHERE id='" . $id . "'";
$result = mysql_query($query) or die(mysql_error());
list($total) = mysql_fetch_row($result);
if ($total == 0) {
  include_once("header.php");
  echo "     <div class=\"error\">Error: There is no bookmark with this ID.</div>\n";
  include_once("footer.php");
  exit;
}

if (!empty($_POST) && isset($_POST['commit'])) {
  foreach ($_POST as $key => $value) {
    $_POST[$key] = mysql_real_escape_string($value);
  }

  $title = $_POST['title'];
  $url = $_POST['url'];

  $titleError = "";
  $urlError = "";

  $valid = true;
  if (empty($title)) {
    $titleError = "Please enter a title for this bookmark.";
    $valid = false;
  } else {
    $title = test_input($title);
  }

  if (empty($url)) {
    $urlError = "Please enter an URL for this bookmark.";
    $valid = false;
  } else {
    $url = test_input($url);

    if (!preg_match("/\b(?:(?:https?|ftp):\/\/|www\.)[-a-z0-9+&@#\/%?=~_|!:,.;]*[-a-z0-9+&@#\/%=~_|]/i", $url)) {
      $urlError = "Please enter a valid URL.";
      $valid = false;
    }
  }

  if ($valid) {
    $query = "UPDATE bookmarks SET url='" . $url . "', title='" . $title . "' WHERE id='" . $id . "'";
    mysql_query($query) or die(mysql_error());

    if (mysql_affected_rows()) {
      $message = "Bookmark has been updated successfully.";
      $_SESSION['msg-success'] = $message;
    }
    header('Location: list.php');
    exit;
  }
} else {
  $query = "SELECT * FROM bookmarks WHERE id='" . $id . "'";
  $result = mysql_query($query);
  $row = mysql_fetch_assoc($result);
  
  $title = $row['title'];
  $url = $row['url'];

  mysql_free_result($result);
}
?>
<?php
include_once("header.php");
?>
    <nav class="navbar navbar-default navbar-fixed-top" role="navigation">
      <div class="container-fluid">
        <div class="navbar-header">
          <a class="navbar-brand" href="#">Bookmarks</a>
        </div>
        <div id="navbar" class="navbar-collapse collapse">
          <ul class="nav navbar-nav navbar-right">
            <li class="dropdown">
              <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                <i class="fa fa-user fa-fw"></i>
                <i class="fa fa-caret-down"></i>
              </a>
              <ul class="dropdown-menu dropdown-user">
                <li><a href="account.php"><i class="fa fa-gear fa-fw"></i> My Account</a></li>
                <li class="divider"></li>
                <li><a href="logout.php"><i class="fa fa-sign-out fa-fw"></i> Log Out</a></li>
              </ul>
            </li>
          </ul>
        </div>
      </div>
    </nav>
<div class="container-fluid">
  <div class="row">
    <div class="col-sm-3 col-md-2 sidebar">
      <ul class="nav nav-sidebar">
        <li class="active">
          <a href="list.php">My bookmarks</a>
        </li>
      </ul>
    </div>
    <div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main">
      <div class="col-md-8">
        <h3 class="page-header">Edit bookmark</h3>
        <form id="edit_bookmark" action="" method="post">
          <div class="form-group<?php echo !empty($titleError) ? ' has-error' : ''; ?>">
            <label for="bookmark-title" class="control-label">Title</label>
            <input type="text" id="bookmark-title" name="title" class="form-control"
                   value="<?php echo stripslashes($title) ?>"/>
            <?php if (!empty($titleError)) { ?>
            <span class="help-block"><?php echo $titleError; ?></span>
            <?php } ?>
          </div>

          <div class="form-group<?php echo !empty($urlError) ? ' has-error' : ''; ?>">
            <label for="bookmark-url" class="control-label">URL</label>
            <input type="url" id="bookmark-url" name="url" class="form-control"
                   value="<?php echo stripslashes($url) ?>"/>
            <?php if (!empty($urlError)) { ?>
            <span class="help-block"><?php echo $urlError; ?></span>
            <?php } ?>
          </div>

          <div class="form-group">
            <div class="pull-right">
              <a class="btn btn-default" href="list.php">Cancel</a>
              <button type="submit" name="commit" class="btn btn-primary">Save</button>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>
</div>
<?php
include_once("footer.php");
?>
