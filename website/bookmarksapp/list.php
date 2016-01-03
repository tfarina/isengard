<?php

include('config.php');

$query = "SELECT * FROM bookmarks ORDER BY dt DESC";
$result = mysql_query($query) or die(mysql_error());

$bookmarks = array();
while ($row = mysql_fetch_assoc($result)) {
  $bookmarks[] = $row;
}

mysql_free_result($result);

include_once("header.php");
?>
<h2>Bookmarks</h2>
<p><a href='new.php'>Create a new bookmark</a></p>
<table class="list">
  <thead>
    <tr>
      <th>URL</th>
      <th>Title</th>
      <th>Date Created</th>
      <th>Actions</th>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($bookmarks as $bookmark) { ?>
    <tr>
      <td><a href="<?php echo $bookmark['url']; ?>"><?php echo $bookmark['url']; ?></a></td>
      <td><?php echo $bookmark['title']; ?></td>
      <td><?php echo $bookmark['dt']; ?></td>
      <td>
        <a href="edit.php?id=<?php echo $bookmark['id']; ?>">Edit</a>  <a href="delete.php?id=<?php echo $bookmark['id']; ?>">Delete</a>
      </td>
    </tr>
    <?php } ?>
  </tbody>
</table>
<?php
include_once("footer.php");
?>
