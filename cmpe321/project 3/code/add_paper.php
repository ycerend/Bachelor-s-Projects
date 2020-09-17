<div class="modal-content" id="myModal" data-toggle="modal" role="document"  style = "text-align: center;vertical-align: middle;width: 650px;height: 3em;margin: auto;">
<?php
require_once("db.php");
session_start();
?>
<div>

          <!-- Horizontal Form -->
          <div class="box box-info">
            <div class="box-header with-border">
              <h3 class="box-title"> Add Paper </h3>
            </div>
            <div class="mypages1" id="response1"></div>

            <!-- /.box-header -->
            <!-- form start -->
            <form class="form-horizontal" id="submit_form" action="add_paper_db.php">
            
            <div class="box-body">
                <div class="form-group">
                  <label for="papername" name="papername" class="col-sm-2 control-label">Paper Title</label>

                  <div class="col-sm-10">
                    <input type="text" class="form-control" name="papername" id="papername">
                  </div>
                </div>
                <div class="form-group">
                  <label for="paperabstract" class="col-sm-2 control-label">Paper Abstract</label>

                  <div class="col-sm-10">
                    <input type="text" class="form-control" id="paperabstract" name="paperabstract" >
                  </div>
                </div>
                <div class="form-group">
                  <label for="paperresult" class="col-sm-2 control-label">Paper Result</label>

                  <div class="col-sm-10">
                    <input type="number" class="form-control" name ="paperresult" id="paperresult" >
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
                  $project_id = (string)$result2->author_id;
                  $class = "checkbox col-sm-2";
                  $input_type = "checkbox";
                  $aname = "a".$project_id;    
                echo "<div class='$class'>
                          <label>
                          <input type='$input_type' name='$aname' > $result2->author_name. $result2->author_surname  <br>
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

                    $class = "checkbox col-sm-2";
                    $input_type = "checkbox";
                    $tname = "t".$result2->topic_id;      
                  echo "<div class='$class'>
                            <label>
                            <input type='$input_type' name='$tname' > $result2->topic_name <br>
                            </label>
                            </div>";
                  }?>
                </div>

                </div>
              <!-- /.box-body -->
              <div class="box-footer">
                <button type="submit" id="myBtn" target="myModal">Submit</button>
              </div>

              <!-- /.box-footer -->
            </form>
    </div>  
    <?php

$user_id =  $_SESSION['user_id'];

  if($user_id === 0){ ?>
      <a href= "admin_menu.php" class="button">Go Menu</a>
  <?php }else{?>
    <a href= "user_menu.php" class="button">Go Menu</a>
<?php }?>


</div>