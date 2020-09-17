
<?php

/**
  * Use an HTML form to create a new entry in the
  * users table.
  *
  */


if (isset($_POST['submit'])) {
  require "db.php";
  function escape($html) {
    return htmlspecialchars($html, ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8");
  }
  $check = true;
  try {
    $topic_name = $_POST['topic_name'];
    $sql = "INSERT INTO `topic_table` (`topic_id`, `topic_name`, `topic_sota`) VALUES (NULL, '$topic_name', 0)";
    $statement = $conn->prepare($sql);
    $statement->execute();
  } catch(PDOException $error) {
    $check = false;
    echo "topic couldn't added due to : " . "<br>" . $error->getMessage();
  }

}
?>
<!DOCTYPE html>
<html lang="en">
<?php if (isset($_POST['submit']) && $statement && $check) { ?>
  > <?php echo $_POST['topic_name']. " successfully added."; ?> 
<?php } ?>


  <body>
  <h2>Add a topic</h2>

<form method="post">
    <label for="topic_name">Topic Name</label>
    <input type="text" name="topic_name" id="topic_name">
    <input type="submit" name="submit" value="Submit">
</form>

<?php
session_start();
$user_id =  $_SESSION['user_id'];

  if($user_id === 0){ ?>
      <a href= "admin_menu.php" class="button">Go Menu</a>
  <?php }else{?>
    <a href= "user_menu.php" class="button">Go Menu</a>
<?php }?>

  </body>
</html>

