<?php
/**
  * Use an HTML form to edit an entry in the
  * users table.
  *
  */
  require "db.php";
  function escape($html) {
    return htmlspecialchars($html, ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8");
  }
if (isset($_POST['submit'])) {
  try {
      $id = $_POST['author_id'];
      $auth_name = $_POST['author_name'];
      $auth_surname  = $_POST['author_surname'];

    $sql = "UPDATE author_table
            SET
              author_name = '$auth_name',
              author_surname = '$auth_surname'
            WHERE author_id = '$id'";

  $statement = $conn->prepare($sql);
  $statement->execute();
  } catch(PDOException $error1) {
      echo $sql . "<br>" . $error1->getMessage();
  }
}

if (isset($_GET['id'])) {
  try {
    $id = $_GET['id'];
    $sql = "SELECT * FROM author_table WHERE author_id = :id";
    $statement = $conn->prepare($sql);
    $statement->bindValue(':id', $id);
    $statement->execute();

    $user = $statement->fetch(PDO::FETCH_ASSOC);
  } catch(PDOException $error) {
      echo $sql . "<br>" . $error->getMessage();
  }
} else {
    echo "Something went wrong!";
    exit;
}
?>
<!DOCTYPE html>
<html lang="en">
<?php if (isset($_POST['submit']) && $statement) : ?>
  <?php echo escape($_POST['author_name'])." successfully updated."; ?> 
<?php endif; ?>

  <body>




<h2>Edit an author</h2>

<form method="post">
    <?php foreach ($user as $key => $value) : if($key !== "author_id"){?>
      <label for="<?php echo $key; ?>"><?php echo ucfirst($key); ?></label>
      <input type="text" name="<?php echo $key; ?>" id="<?php echo $key; ?>" value="<?php echo escape($value); ?>" <?php echo ($key === 'id' ? 'readonly' : null); ?>>
    <?php }else{?>
            <input type="hidden" name="<?php echo $key; ?>" id="<?php echo $key; ?>" value="<?php echo escape($value); ?>" <?php echo ($key === 'id' ? 'readonly' : null); ?>>

    <?php } endforeach; ?>
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