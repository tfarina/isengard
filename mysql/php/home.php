<?
include("distwi.php");

if (!isLoggedIn()) {
    header("Location: index.php");
    exit;
}

include("header.php");
?>

<a href="logout.php">logout</a>

<?
include("footer.php")
?>
