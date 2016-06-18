<?php

require_once 'config.php';

unset($_SESSION['msg-success']);

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

  $titleError = "";
  $urlError = "";

  $title = "";
  $url = "";

  $valid = true;
  if (empty($_POST['title'])) {
    $titleError = "Please enter a title for this bookmark.";
    $valid = false;
  } else {
    $title = test_input($_POST['title']);
  }

  if (empty($_POST['url'])) {
    $urlError = "Please enter an URL for this bookmark.";
    $valid = false;
  } else {
    $url = test_input($_POST['url']);

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
    } else {
      $message = "Nothing has changed.";
    }
    //header('Location: list.php?msg=' . urlencode($message));
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
    <nav class="navbar navbar-inverse navbar-fixed-top">
      <div class="container-fluid">
        <div class="navbar-header">
          <a class="navbar-brand" href="#">Bookmarks</a>
        </div>
        <div id="navbar" class="navbar-collapse collapse">
          <ul class="nav navbar-nav navbar-right">
            <li><a href="#">My Account</a></li>
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
      <?php if (isset($message)) { ?>
      <div class="alert alert-success" role="alert"><?php echo $message; ?></div>
      <?php } ?>
      <h3 class="page-header">Edit bookmark</h3>
  <form action="" method="post" class="form-horizontal">
    <div class="form-group <?php echo !empty($titleError) ? 'has-error' : ''; ?>">
      <label for="bookmark-title" class="col-sm-2 control-label">Title</label>
      <div class="col-sm-4">
        <input type="text" name="title" id="bookmark-title" class="form-control"
               value="<?php echo stripslashes($title) ?>"/>
        <?php if (!empty($titleError)) { ?>
        <span class="help-block"><?php echo $titleError; ?></span>
        <?php } ?>
      </div>
    </div>

    <div class="form-group <?php echo !empty($urlError) ? 'has-error' : ''; ?>">
      <label for="bookmark-url" class="col-sm-2 control-label">URL</label>
      <div class="col-sm-4">
        <input type="url" name="url" id="bookmark-url" class="form-control"
               value="<?php echo stripslashes($url) ?>"/>
        <?php if (!empty($urlError)) { ?>
        <span class="help-block"><?php echo $urlError; ?></span>
        <?php } ?>
      </div>
    </div>

    <div class="form-group">
      <div class="col-sm-offset-2 col-sm-4">
        <div class="pull-right">
          <a class="btn btn-default" href="list.php">Cancel</a>
          <button type="submit" name="commit" class="btn btn-primary">Save</button>
        </div>
      </div>
    </div>
  </form>
    </div>
  </div>
</div>
<?php
include_once("footer.php");
?>
