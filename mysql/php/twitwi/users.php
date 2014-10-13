<?

session_start();

include("db.php");
include("functions.php");

?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta http-equiv="content-type" content="text/html"; charset=utf-8 />
  <title>MicroBlogging App</title>
</head>
</body>
  <h1>List of Users</h1>
  <?
  $users = show_users();

  if (count($users)) {
  ?>
  <table border="1" cellspacing="0" cellpading="5" width="500">
  <?
    foreach ($users as $key => $value) {
      echo "<tr valgin='top'>\n";
      #echo "<td>". $key ."</td>\n";
      echo "<td>". $value."</td>\n";
      echo "<td><a href='#'>follow</a></td>\n";
      echo "</tr>\n";
    }
  ?>
  </table>
  <?
  } else {
  ?>
  <p><b>There are no users in the system!</b></p>
  <?
  }
  ?>
</body>
</html>
