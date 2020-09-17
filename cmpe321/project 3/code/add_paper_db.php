<?php
session_start();
require_once("db.php");

    try {
        if (isset($_GET['papername'])) {
            $paper_name = $_GET['papername']; 
        }
        if (isset($_GET['paperabstract'])) {
            $paper_abstract = $_GET['paperabstract']; 
        }
        if (isset($_GET['paperresult'])) {
            $paper_result = $_GET['paperresult']; 
        }
        $sql_get = "INSERT INTO `paper_table`(`paper_id`, `paper_title`, `paper_abstract`, `paper_result`) VALUES (NULL,'$paper_name','$paper_abstract','$paper_result')";
        $statement2 = $conn->prepare($sql_get);
        $statement2->execute();
    } catch(PDOException $error) {
        echo "Something went wrong!";
    }
    $paper_id = 0 ;
    try {
        if (isset($_POST['papername'])) {
            $paper_name = $_GET['papername']; 
        }
        $sql_get = "SELECT paper_id FROM `paper_table` WHERE paper_title ='$paper_name';";
        $statement2 = $conn->prepare($sql_get);
        $statement2->execute();
        while($result = $statement2->fetchObject()){
            $paper_id = $result->paper_id;
        }
    } catch(PDOException $error) {
        echo "Something went wrong!";
    }
    try{
        if($paper_id !== 0){
            $sql_get = "SELECT author_id FROM `author_table`";
            $statement2 = $conn->prepare($sql_get);
            $statement2->execute();
            $sql_get = "";
            while($result = $statement2->fetchObject()){
                
                $author_id = $result->author_id;
                $temp = "a".$author_id;
                if (isset($_GET[$temp])) {
                    $sql_get = $sql_get. "INSERT INTO `author_paper_table`(`ap_id`, `p_id`, `a_id`) VALUES (NULL,'$paper_id','$author_id' );";

                }
            }
            $statement2 = $conn->prepare($sql_get);
            $statement2->execute();


    } 
  }
  catch(PDOException $error) {
    echo "Something went wrong!";
}
try{
    if($paper_id !== 0){
        $sql_get = "SELECT topic_id FROM `topic_table`";
        $statement2 = $conn->prepare($sql_get);
        $statement2->execute();
        $sql_get = "";
        while($result = $statement2->fetchObject()){
            $topic_id = $result->topic_id;
            $temp = "t".$topic_id;
            if (isset($_GET[$temp])) {
                $sql_get = $sql_get. "INSERT INTO `topic_paper_table`(`tp_id`, `p_id`, `t_id`) VALUES (NULL,'$paper_id','$topic_id' );";

            }
        }
        $statement2 = $conn->prepare($sql_get);
        $statement2->execute();


} 
echo $paper_name." is successfully added!";
}
catch(PDOException $error) {
echo "Something went wrong!";
}


?>
<?php

$user_id =  $_SESSION['user_id'];

  if($user_id === 0){ ?>
      <a href= "admin_menu.php" class="button">Go Menu</a>
  <?php }else{?>
    <a href= "user_menu.php" class="button">Go Menu</a>
<?php }?>
