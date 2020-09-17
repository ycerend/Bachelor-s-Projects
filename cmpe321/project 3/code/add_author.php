
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
    $author_name = $_POST['firstname'];
    $author_surname = $_POST['lastname'];
    $sql = "INSERT INTO `author_table` (`author_id`, `author_name`, `author_surname`) VALUES (NULL, '$author_name', '$author_surname')";
    $statement = $conn->prepare($sql);
    $statement->execute();
  } catch(PDOException $error) {
    $check = false;
    echo "author couldn't added due to : " . "<br>" . $error->getMessage();
  }

}
?>
<!DOCTYPE html>
<html lang="en">
<?php if (isset($_POST['submit']) && $statement && $check) { ?>
  > <?php echo $_POST['firstname']. " successfully added."; ?> 
<?php } ?>


  <body>
  <h2>Add a author</h2>

<form method="post">
    <label for="firstname">Author Name</label>
    <input type="text" name="firstname" id="firstname">
    <label for="lastname">Author  Surname</label>
    <input type="text" name="lastname" id="lastname">
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

