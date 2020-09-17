
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
    $sql = "SELECT author_name,author_surname, COUNT(*) AS number_of_sota
    FROM author_table,author_paper_table,paper_table
                                    WHERE author_table.author_id = author_paper_table.a_id AND 
                                    author_paper_table.p_id = paper_table.paper_id 
                                   GROUP BY author_name,author_surname 
                                   ORDER BY COUNT(*) DESC
    ";

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
  <h2>View Authors</h2>

<table>
  <thead>
    <tr>
      <th>First Name</th>
      <th>Last Name</th>
      <th>Number Of Sota</th>
    </tr>
  </thead>
    <tbody>
    <?php foreach ($result as $row) : ?>
      <tr>
        <td><?php echo escape($row["author_name"]); ?></td>
        <td><?php echo escape($row["author_surname"]); ?></td>
        <td><?php echo escape($row["number_of_sota"]); ?></td>
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

