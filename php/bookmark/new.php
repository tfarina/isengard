<?php

require_once 'config.php';

if (!empty($_POST) && isset($_POST['submit'])) {
  foreach($_POST as $key => $value) {
    $data[$key] = mysql_real_escape_string($value);
  }

  $title = $data['title'];
  $url = $data['url'];

  $titleError = "";
  $urlError = "";

  $valid = true;
  if (empty($title)) {
    $titleError = "Please enter a title for this bookmark.";
    $valid = false;
  }

  if (empty($url)) {
    $urlError = "Please enter an URL for this bookmark.";
    $valid = false;
  }

  if ($valid) {
    $query = "INSERT INTO bookmarks (title, url) VALUES ";
    $query .= "('$title', '$url')";
    mysql_query($query) or die(mysql_error());

    $message = "Bookmark has been created successfully.";
    $_SESSION['msg-success'] = $message;
    //header('Location: list.php?msg=' . urlencode($message));
    header('Location: list.php');
    exit;
  }
}

?>
<?php
include_once("header.php");
?>
  <nav class="navbar navbar-default navbar-fixed-top" role="navigation">
    <div class="container-fluid">
      <div class="navbar-header">
        <button type="button" class="navbar-toggle collapsed" data-toggle="collapse"
                data-target="#navbar" aria-expanded="false" aria-controls="navbar">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
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
          <h3 class="page-header">Create New Bookmark</h3>
          <form id="new_bookmark" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']); ?>"
                method="post">
            <div class="form-group <?php echo !empty($titleError) ? 'has-error' : ''; ?>">
              <label for="bookmark-title" class="control-label">Title</label>
              <input type="text" id="bookmark-title" name="title" class="form-control" />
              <?php if (!empty($titleError)) { ?>
              <span class="help-block"><?php echo $titleError; ?></span>
              <?php } ?>
            </div>

            <div class="form-group <?php echo !empty($urlError) ? 'has-error' : ''; ?>">
              <label for="bookmark-url" class="control-label">URL</label>
              <input type="url" id="bookmark-url" name="url" class="form-control" />
              <?php if (!empty($urlError)) { ?>
              <span class="help-block"><?php echo $urlError; ?></span>
              <?php } ?>
            </div>

            <div class="form-group">
              <div class="pull-right">
                <a class="btn btn-default" href="list.php">Cancel</a>
                <button type="submit" name="submit" class="btn btn-primary">Create</button>
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
