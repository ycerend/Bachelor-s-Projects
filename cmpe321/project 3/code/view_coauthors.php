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
                    <th>Author Name</th>
                    <th>Author Surname</th>

                </tr>
                </thead>
                <tbody>
                <?php
                require_once("db.php");
                if (isset($_GET['name']) && $_GET['surname']) {
                    try {
                    $name = $_GET['name'];
                    $surname = $_GET['surname'];
                    $sql2="CALL getCoAuthorsOfAnAuthor('$name','$surname') ";
                                
                      $statement2 = $conn->prepare($sql2);
                      $statement2->execute();

                
                while($result = $statement2->fetchObject()){  ?>
                    <tr>
                        <td><?php echo $result->author_name; ?></td>
                        <td><?php echo $result->author_surname; ?></td>          
                    </tr>
                    <?php } }                       
                      catch(PDOException $error) {
                      echo $sql2 . "<br>" . $error->getMessage();
                      } }else {
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
