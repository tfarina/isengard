<?php

include('config.php');

if (isset($_POST['submit'])) {
  foreach($_POST as $key => $value) {
    $data[$key] = mysql_real_escape_string($value);
  }

  $title = $data['title'];
  $url = $data['url'];

  $valid = true;
  if (empty($title)) {
    $valid = false;
  }

  if ($valid) {
    $query = "INSERT INTO bookmarks (title, url) VALUES ";
    $query .= "('$title', '$url')";
    mysql_query($query) or die(mysql_error());

    $message = "Bookmark has been created successfully.";
    header('Location: list.php?msg=' . urlencode($message));
    exit;
  }
}

?>
<?php
include_once("header.php");
?>
  <?php if (isset($message)) { ?>
  <div class="message">
    <?php echo $message; ?>
  </div>
  <?php } ?>
  <h3>Create New Bookmark</h3>
  <form name="newBookmark" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
    <table>
      <tbody>
      <tr>
        <td align="right">Title:</td>
        <td><input type="text" name="title"/></td>
      </tr>
      <tr>
        <td align="right">URL:</td>
        <td><input type="text" name="url"/></td>
      </tr>
      <tr>
        <td></td>
        <td style="float: right;">
          <a class="btn btn-default" href="list.php">Cancel</a>
          <button type="submit" name="submit" class="btn btn-primary">Create</button>
        </td>
      </tr>
      </tbody>
    </table>
  </form>
<div class="container">
  <div class="col-sm-offset-9 col-sm-8">
  <div class="panel panel-default">
    <div class="panel-heading">
      Create new bookmark
    </div>
    <div class="panel-body">
      <form action="" method="post" class="form-horizontal">
        <div class="form-group">
          <label for="bookmark-title" class="col-sm-3 control-label">Title</label>
          <div class="col-sm-6">
            <input type="text" name="title" id="bookmark-title" class="form-control">
          </div>
        </div>
      </form>
    </div> 
  </div>        
</div> 
</div>        

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
