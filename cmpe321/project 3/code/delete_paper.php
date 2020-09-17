<?php
require_once("db.php");
if (isset($_GET['id'])) {
    try {

    $id = $_GET['id'];
    $sql2="DELETE FROM paper_table WHERE paper_id=$id ";
    $updateCheck = $conn->query($sql2);
      if($updateCheck ){
        echo "Deleted succesfully!";
    }
    else{
        echo "Something went wrong!";
    }
      } 
      catch(PDOException $error) {
      echo $sql2 . "<br>" . $error->getMessage();
      }
  } else {
      echo "Something went wrong!";
  }
  
?>
<?php
session_start();
$user_id =  $_SESSION['user_id'];

  if($user_id === 0){ ?>
      <a href= "admin_menu.php" class="button">Go Menu</a>
  <?php }else{?>
    <a href= "user_menu.php" class="button">Go Menu</a>
<?php }?>
