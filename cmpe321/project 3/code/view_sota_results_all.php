
<?php

/**
  * Use an HTML form to create a new entry in the
  * users table.
  *
  */

  require "db.php";
  function escape($html) {
    return htmlspecialchars($html, ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8");
  }
  try {
    $sql = "SELECT * FROM topic_table";

    $statement = $conn->prepare($sql);
    $statement->execute();
  
    $result = $statement->fetchAll();
  } catch(PDOException $error) {
    echo $sql . "<br>" . $error->getMessage();
  }
?>
<!DOCTYPE html>
<html lang="en">

  <body>
  <h2>Edit Topic</h2>

<table>
  <thead>
    <tr>
      <th>Topic Name</th>
      <th>Topic Sota</th>
      <th>View Paper(s) of Related SOTA Result</th>
    </tr>
  </thead>
    <tbody>
    <?php foreach ($result as $row) : ?>
      <tr>
        <td><?php echo escape($row["topic_name"]); ?></td>
        <td><?php echo escape($row["topic_sota"]); ?></td>
        <td><a href="view_paper_of_sota.php?id=<?php echo escape($row["topic_id"])."&"; ?>sota=<?php echo escape($row["topic_sota"]); ?>">View</a></td>
      </tr>
    <?php endforeach; ?>
    </tbody>
</table>


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

