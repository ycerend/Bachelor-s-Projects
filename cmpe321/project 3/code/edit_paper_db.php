<?php
require_once("db.php");

    try {
        $sql_get = "";
        if (isset($_GET['id'])) {
            $paper_id = $_GET['id']; 
        }
        if (isset($_GET['papername'])) {
            $paper_name = $_GET['papername']; 
            $sql_get = "UPDATE `paper_table` SET `paper_title` ='$paper_name' WHERE `paper_id`= '$paper_id';";

        }
        if (isset($_GET['paperabstract'])) {
            $paper_abstract = $_GET['paperabstract']; 
            $sql_get = $sql_get. "UPDATE `paper_table` SET `paper_abstract` ='$paper_abstract' WHERE `paper_id`= '$paper_id';";

        }
        if (isset($_GET['paperresult'])) {
            $paper_result = $_GET['paperresult']; 
            $sql_get = $sql_get. "UPDATE `paper_table` SET `paper_result` ='$paper_result' WHERE `paper_id`= '$paper_id';";

        }
        $statement2 = $conn->prepare($sql_get);
        $statement2->execute();
    } catch(PDOException $error) {
        echo $sql_get . "<br>" . $error->getMessage();
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
                $sql4="SELECT * FROM author_paper_table WHERE p_id = '$paper_id'  AND a_id ='$author_id' ";
                $statement3 = $conn->prepare($sql4);
                $statement3->execute();
                if (isset($_GET[$temp])) {

                    if($result3 = $statement3->fetchObject()){ 
                      }else{
                        $sql_get = $sql_get. "INSERT INTO `author_paper_table`(`ap_id`, `p_id`, `a_id`) VALUES (NULL,'$paper_id','$author_id' ) ;";
                      }
                }else{
                    if($result3 = $statement3->fetchObject()){ 
                    
                        $sql_get = $sql_get. "DELETE FROM `author_paper_table` WHERE p_id = '$paper_id' AND a_id ='$author_id';";
                    }
                }
            }
            if($sql_get !== ""){
                $statement2 = $conn->prepare($sql_get);
                $statement2->execute();
            }



    } 
  }
  catch(PDOException $error) {
    echo $sql_get . "<br>" . $error->getMessage();
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
            $sql4="SELECT * FROM topic_paper_table WHERE p_id = '$paper_id'  AND t_id ='$topic_id' ";
            $statement3 = $conn->prepare($sql4);
            $statement3->execute();
            if (isset($_GET[$temp])) {
                if($result3 = $statement3->fetchObject()){ 
                }else{
                    $sql_get = $sql_get. "INSERT INTO `topic_paper_table`(`tp_id`, `p_id`, `t_id`) VALUES (NULL,'$paper_id','$topic_id' );";
                }

            }else{
                if($result3 = $statement3->fetchObject()){ 
                    $sql_get = $sql_get. "DELETE FROM `topic_paper_table` WHERE p_id = '$paper_id' AND t_id ='$topic_id'";
                
                }else{
                }
            }
        }

        if($sql_get !== ""){
            $statement2 = $conn->prepare($sql_get);
            $statement2->execute();
        }


}
echo $paper_name." edited successfully";
}
catch(PDOException $error) {
echo $sql_get . "<br>" . $error->getMessage();
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

