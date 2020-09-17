<body class="hold-transition skin-blue sidebar-mini">


    <!-- Main content -->
    <section class="content">
          <div class="box">
            <div class="box-header">
              <h3 class="box-title">Paper List</h3>
            </div>
            <!-- /.box-header -->
            <div class="box-body no-padding">
              <table class="table table-striped" id= "user_list">
              <thead>
                <tr>
                    <th>Paper Title </th>
                    <th>Paper Abstract</th>
                    <th>Paper Result</th>
                    <th>Authors </th>
                    <th>Topics</th>
                </tr>
                </thead>
                <tbody>
                <?php
                require_once("db.php");
                if (isset($_GET['id'])) {
                    try {
                
                    $id = $_GET['id'];
                    $sql2="SELECT * FROM paper_table,topic_paper_table WHERE paper_table.paper_id = topic_paper_table.p_id AND topic_paper_table.t_id='$id' ";
                                
                      $statement2 = $conn->prepare($sql2);
                      $statement2->execute();
                      } 
                      catch(PDOException $error) {
                      echo $sql2 . "<br>" . $error->getMessage();
                      }
                
                while($result = $statement2->fetchObject()){ ?>
                    <tr>
                        <td><?php echo $result->paper_title; ?></td>
                        <td><?php echo $result->paper_abstract; ?></td>
                        <td><?php echo $result->paper_result; ?></td>
                        <td><?php
                            $sql3 = "SELECT author_table.author_name, author_surname
                                FROM author_table,author_paper_table,paper_table
                                WHERE author_table.author_id = author_paper_table.a_id AND 
                                author_paper_table.p_id = paper_table.paper_id AND paper_table.paper_id = '$result->paper_id'";
                                $statement3 = $conn->prepare($sql3);
                                $statement3->execute();
                                while($result3 = $statement3->fetchObject()){
                                    echo $result3->author_name." ".$result3->author_surname . " <br />"; 
                                }?> </td>
                        <td><?php
                            $sql4= "SELECT topic_table.topic_name
                                FROM topic_table,topic_paper_table,paper_table
                                WHERE topic_table.topic_id = topic_paper_table.t_id AND 
                                topic_paper_table.p_id = paper_table.paper_id AND paper_table.paper_id = '$result->paper_id'";
                                $statement4 = $conn->prepare($sql4);
                                $statement4->execute();
                                while($result4 = $statement4->fetchObject()){
                                    echo $result4->topic_name. " <br />"; 
                                }?> </td>
                    </tr>
                    <?php } } else {
                      echo "Something went wrong!2";
                  } ?> 
                </tbody>
              </table>
            </div>
            <!-- /.box-body -->
          </div>
          <!-- /.box -->
    </div>
    <!-- <iframe class="modal fade in"  style="display: block; padding-left: 0px;" name="my_modal"></iframe> -->
    <div id="myModal" class=" modal" style="  padding-top: 20px;
  padding-right: 30px;
  padding-bottom: -900px;
  padding-left: 80px;" role="document">
    </div>


</body>

<?php
session_start();
$user_id =  $_SESSION['user_id'];

  if($user_id === 0){ ?>
      <a href= "admin_menu.php" class="button">Go Menu</a>
  <?php }else{?>
    <a href= "user_menu.php" class="button">Go Menu</a>
<?php }?>
