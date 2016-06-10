<?php

require_once 'config.php';

if (!isUserLoggedIn()) {
  header('Location: login.php');
  exit;
}

$id = $_SESSION['userid'];

$query = "SELECT * FROM user WHERE user_id = '$id';";
$result = mysql_query($query) or die(mysql_error());
$row = mysql_fetch_assoc($result);
mysql_free_result($result);
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
      <li><a href="logout.php">Log Out</a></li>
    </ul>
  </div>
</div>
</nav>

<div class="container">
  <div class="col-sm-12">
    <div class="form-group col-sm-12">
      <label class="col-sm-2 control-label">Full name</label>
      <div class="col-sm-10">
          <?php echo $row['fullname']; ?>
      </div>
    </div>
    <div class="form-group col-sm-12">
      <label class="col-sm-2 control-label">Email</label>
      <div class="col-sm-10">
          <?php echo $row['email']; ?>
      </div>
    </div>
  </div>
</div>
<?php
include_once("footer.php");
?>
