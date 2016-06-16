<?php

require_once 'config.php';

unset($_SESSION['msg-success']);

if (!empty($_POST) && isset($_POST['submit'])) {
  foreach($_POST as $key => $value) {
    $data[$key] = mysql_real_escape_string($value);
  }

  $title = $data['title'];
  $url = $data['url'];

  $valid = true;
  if (empty($title)) {
    $valid = false;
  }

  if (empty($url)) {
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
  <h3>Create New Bookmark</h3>
  <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post" class="form-horizontal">
    <div class="form-group <?php echo !empty($titleError) ? 'has-error' : ''; ?>">
      <label for="bookmark-title" class="col-sm-2 control-label">Title</label>
      <div class="col-sm-4">
        <input type="text" name="title" id="bookmark-title" class="form-control" />
        <?php if (!empty($titleError)) { ?>
        <span class="help-block"><?php echo $titleError; ?></span>
        <?php } ?>
      </div>
    </div>

    <div class="form-group <?php echo !empty($urlError) ? 'has-error' : ''; ?>">
      <label for="bookmark-url" class="col-sm-2 control-label">URL</label>
      <div class="col-sm-4">
        <input type="url" name="url" id="bookmark-url" class="form-control" />
        <?php if (!empty($urlError)) { ?>
        <span class="help-block"><?php echo $urlError; ?></span>
        <?php } ?>
      </div>
    </div>

    <div class="form-group">
      <div class="col-sm-offset-2 col-sm-4">
        <div class="pull-right">
          <a class="btn btn-default" href="list.php">Cancel</a>
          <button type="submit" name="submit" class="btn btn-primary">Create</button>
        </div>
      </div>
    </div>
  </form>
<?php
include_once("footer.php");
?>
