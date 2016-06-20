<?php

require_once 'config.php';

if (!is_user_logged_in()) {
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

<div class="container">
  <div class="col-sm-12">
    <h3 class="sub-header">Account Settings</h3>
    <div class="form-group col-sm-12">
      <label class="col-sm-2 control-label">Name</label>
      <div class="col-sm-10">
        <?php echo $row['fullname']; ?>
      </div>
    </div>
    <div class="form-group col-sm-12">
      <label class="col-sm-2 control-label">E-mail</label>
      <div class="col-sm-10">
        <?php echo $row['email']; ?>
      </div>
    </div>
  </div>
</div>
<?php
include_once("footer.php");
?>
