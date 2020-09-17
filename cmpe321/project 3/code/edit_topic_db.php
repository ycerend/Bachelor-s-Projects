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
  $check = true;
if (isset($_POST['submit'])) {

  try {
      $id = $_POST['topic_id'];
      $topic_name = $_POST['topic_name'];
      $topic_sota  = $_POST['topic_sota'];

    $sql = "UPDATE topic_table
            SET
            topic_name = '$topic_name',
            topic_sota = '$topic_sota'
            WHERE topic_id = '$id'";

  $statement = $conn->prepare($sql);
  $statement->execute();
  } catch(PDOException $error1) {
    $check = false;
    echo "topic couldn't be updated due to : " . "<br>" . $error->getMessage();
  }
}

if (isset($_GET['id'])) {
  try {
    
    $id = $_GET['id'];
    $sql = "SELECT * FROM topic_table WHERE topic_id = :id";
    $statement = $conn->prepare($sql);
    $statement->bindValue(':id', $id);
    $statement->execute();

    $user = $statement->fetch(PDO::FETCH_ASSOC);
  } catch(PDOException $error) {
    $check = false;
    echo "topic couldn't be updated due to : " . "<br>" . $error->getMessage();
  }
} else {
  $check = false;
  echo "topic couldn't be updated due to : " . "<br>" . $error->getMessage();
}
?>
<!DOCTYPE html>
<html lang="en">
<?php if (isset($_POST['submit']) && $statement &&$check) : ?>
  <?php echo escape($_POST['topic_name'])." successfully updated."; ?> 
<?php endif; ?>

  <body>




<h2>Edit a topic </h2>

<form method="post">
    <?php foreach ($user as $key => $value) : if($key !== "topic_id" && $key !== "topic_sota"){?>
      <label for="<?php echo $key; ?>"><?php echo ucfirst($key); ?></label>
      <input type="text" name="<?php echo $key; ?>" id="<?php echo $key; ?>" value="<?php echo escape($value); ?>" <?php echo ($key === 'id' ? 'readonly' : null); ?>>
    <?php }else if($key == "topic_sota"){?>
      <label for="<?php echo $key; ?>"><?php echo ucfirst($key); ?></label>
            <input type="text" name="<?php echo $key; ?>" id="<?php echo $key; ?>" value="<?php echo escape($value); ?>"  <?php echo ($key !== 'id' ? 'readonly' : null); ?> >

    <?php } else{?>
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
readonly