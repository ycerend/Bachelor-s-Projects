<?php
require_once("db.php");
session_start();
if (isset($_GET['id'])) {
    try {

    $id = $_GET['id'];
    $sql2="SELECT * FROM paper_table WHERE paper_id='$id' ";
                
      $statement2 = $conn->prepare($sql2);
      $statement2->execute();
      } 
      catch(PDOException $error) {
      echo $sql2 . "<br>" . $error->getMessage();
      }
  } else {
      echo "Something went wrong!2";
  }
?>
<div>
            <!-- /.box-header -->
            <!-- form start -->
            <?php while($result = $statement2->fetchObject()){ ?>
            <form class="form-horizontal" id="submit_form" action="edit_paper_db.php?id=<?php echo  $id;?>">
            
            <div class="box-body">
                <div class="form-group">
                
                  <label for="papername" name="papername" class="col-sm-2 control-label">Paper Title</label>

                  <div class="col-sm-10">
                  <input type="hidden" value="<?php echo $result->paper_id; ?>" name="id" />
                    <input type="text" class="form-control" name="papername"  id="papername" value="<?php echo $result->paper_title; ?>">
                  </div>
                </div>
                <div class="form-group">
                  <label for="paperabstract" class="col-sm-2 control-label">Paper Abstract</label>

                  <div class="col-sm-10">
                    <input type="text" class="form-control" id="paperabstract" name="paperabstract" value="<?php echo $result->paper_abstract; ?>" >
                  </div>
                </div>
                <div class="form-group">
                  <label for="paperresult" class="col-sm-2 control-label">Paper Result</label>

                  <div class="col-sm-10">
                    <input type="number" class="form-control" name ="paperresult" id="paperresult" value="<?php echo $result->paper_result; ?>">
                  </div>
                </div>

               
               <div class="form-group">
                  <label class="col-sm-2 control-label">Authors (Pick Authors or Add New Author first) :</label>

                  <?php
                    try{
                $sql2="SELECT * FROM author_table";
                
                $statement2 = $conn->prepare($sql2);
                $statement2->execute();
                } catch(PDOException $error2) {
                echo $sql2 . "<br>" . $error2->getMessage();
                }   

                while($result2 = $statement2->fetchObject()){
                  $checked = "";
                  $project_id = (string)$result2->author_id;
                  $class = "checkbox col-sm-2";
                  $input_type = "checkbox";
                  $aname = "a".$project_id;
                  $sql3="SELECT * FROM author_paper_table WHERE p_id = '$id'  AND a_id ='$project_id' ";
                  $statement3 = $conn->prepare($sql3);
                  $statement3->execute();
                  if($result3 = $statement3->fetchObject()){ 
                    $checked = "checked"; 
                    }     
                echo "<div class='$class'>
                          <label>
                          <input type='$input_type' name='$aname' $checked> $result2->author_name. $result2->author_surname  <br>
                          </label>
                          </div>";
                }?>
                </div>
                <div class="form-group">
                  <label class="col-sm-2 control-label">Topics</label>

                  <?php
                    try{
                $sql2="SELECT * FROM topic_table";                
                $statement2 = $conn->prepare($sql2);
                $statement2->execute();
                } catch(PDOException $error2) {
                echo $sql2 . "<br>" . $error2->getMessage();
                }   

                while($result2 = $statement2->fetchObject()){
                    $checked = "";
                    $class = "checkbox col-sm-2";
                    $input_type = "checkbox";
                    $project_id = $result2->topic_id; 
                    $tname = "t".$result2->topic_id;    
                    $sql3="SELECT * FROM topic_paper_table WHERE p_id = '$id'  AND t_id ='$project_id' ";
                    $statement3 = $conn->prepare($sql3);
                    $statement3->execute();
                    if($result3 = $statement3->fetchObject()){ 
                      $checked = "checked"; 
                      }  
                  echo "<div class='$class'>
                            <label>
                            <input type='$input_type' name='$tname' $checked> $result2->topic_name <br>
                            </label>
                            </div>";
                  }?>
                </div>
                <?php }?>
                </div>
              <!-- /.box-body -->
              <div class="box-footer">
                <button type="submit" id="myBtn" target="myModal">Submit</button>
              </div>

              <!-- /.box-footer -->
            </form>
        </div>  <?php

$user_id =  $_SESSION['user_id'];

  if($user_id === 0){ ?>
      <a href= "admin_menu.php" class="button">Go Menu</a>
  <?php }else{?>
    <a href= "user_menu.php" class="button">Go Menu</a>
<?php }?>
