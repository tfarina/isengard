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
      <ul class="nav nav-pills nav-stacked">
        <li class="nav-item"><a class="nav-link" href="#">Overview</a></li>
	<li class="nav-item"><a class="nav-link" href="#">Reports</a></li>
	<li class="nav-item"><a class="nav-link" href="#">Analytics</a></li>
	<li class="nav-item"><a class="nav-link" href="#">Export</a></li>
	<li class="nav-item"><a class="nav-link" href="">Nav item</a></li>
	<li class="nav-item"><a class="nav-link" href="">Nav item</a></li>
	<li class="nav-item"><a class="nav-link" href="">Another</a></li>
	<li class="nav-item"><a class="nav-link" href="">Nav item</a></li>
	<li class="nav-item"><a class="nav-link" href="">One more</a></li>
      </ul>
    </div>
    <div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main">
      <?php if (isset($_GET['msg'])) { ?>
      <div class="alert alert-success" role="alert"><?php echo $_GET['msg']; ?></div>
      <?php } ?>
      <?php if (isset($_SESSION['msg-success'])) { ?>
      <div class="alert alert-success alert-dismissible" role="alert">
        <button type="button" class="close" data-dismiss="alert" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
        <?php echo $_SESSION['msg-success']; unset($_SESSION['msg-success']); ?>
      </div>
      <?php } ?>
      <?php if (count($bookmarks) == 0) { ?>
      <!--<div><center>You have no bookmarks yet.</center></div>-->
      <div class="panel panel-default">
        <div class="panel-heading">
          <h3 class="panel-title">Bookmarks</h3>
        </div>
        <div class="panel-body">
          <span>You have no bookmarks yet. Get started by adding a bookmark.</span>
          <div style="margin-top:10px;">
            <a href="new.php">
              <i class="fa fa-plus fa-fw"></i> Add Bookmark
            </a>
          </div>
        </div>
      </div>
      <?php } else { ?>
      <p>
        <a href="new.php" class="btn btn-success">
          <i class="fa fa-plus fa-fw"></i> Add Bookmark
        </a>
      </p>
      <table class="table table-hover">
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
              <a href="edit.php?id=<?php echo $bookmark['id']; ?>" class="btn btn-sm btn-primary">
                <i class="fa fa-pencil fa-fw"></i> Edit </a>  <a href="delete.php?id=<?php echo $bookmark['id']; ?>" class="btn btn-sm btn-danger">
                <i class="fa fa-trash-o fa-fw"></i> Delete
              </a>
            </td>
          </tr>
          <?php } ?>
        </tbody>
      </table>
      <?php } ?>
    </div>
  </div>
</div>
<?php
include_once("footer.php");
?>
