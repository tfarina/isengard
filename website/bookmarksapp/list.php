<?php

require_once 'config.php';

$query = "SELECT * FROM bookmarks ORDER BY dt DESC";
$result = mysql_query($query) or die(mysql_error());

$bookmarks = array();
while ($row = mysql_fetch_assoc($result)) {
  $bookmarks[] = $row;
}

mysql_free_result($result);

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
      <?php if (isset($_GET['msg'])) { ?>
      <div class="alert alert-success" role="alert"><?php echo $_GET['msg']; ?></div>
      <?php } ?>
      <?php if (isset($_SESSION['msg-success'])) { ?>
      <div class="alert alert-success" role="alert">
        <a href="#" class="close" data-dismiss="alert" aria-label="close">&times;</a>
        <?php echo $_SESSION['msg-success']; ?>
      </div>
      <?php } ?>
      <p><a href="new.php" class="btn btn-success">Add bookmark</a></p>
      <table class="table table-striped table-bordered table-hover">
        <thead>
          <tr>
            <th>Title</th>
            <th>URL</th>
            <th>Date Created</th>
            <th>Actions</th>
          </tr>
        </thead>
        <tbody>
          <?php foreach ($bookmarks as $bookmark) { ?>
          <tr>
            <td><a href="<?php echo $bookmark['url']; ?>"><?php echo $bookmark['title']; ?></a></td>
            <td><?php echo $bookmark['url']; ?></td>
            <td><?php echo $bookmark['dt']; ?></td>
            <td>
              <a href="edit.php?id=<?php echo $bookmark['id']; ?>">Edit</a>  <a href="delete.php?id=<?php echo $bookmark['id']; ?>">Delete</a>
            </td>
          </tr>
          <?php } ?>
        </tbody>
      </table>
    </div>
  </div>
</div>
<?php
include_once("footer.php");
?>
