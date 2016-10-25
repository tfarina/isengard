<div id="navbar">
  <a href="index.php">home</a>
  <?
    if (isLoggedIn()) {
  ?>
  | <a href="logout.php">logout</a>
  <?
    }
  ?>
</div>
