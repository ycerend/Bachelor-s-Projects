
<div>
            <!-- /.box-header -->
            <!-- form start -->
            <form class="form-horizontal" id="submit_form" action="view_coauthors.php">
            
            <div class="box-body">
                <div class="form-group">
                
                  <label for="name" name="name" class="col-sm-2 control-label">Author Name</label>

                  <div class="col-sm-10">
                    <input type="text" class="form-control" name="name"  id="name" >
                  </div>
                </div>
                <div class="form-group">
                
                <label for="surname" name="surname" class="col-sm-2 control-label">Author Surname</label>

                <div class="col-sm-10">
                  <input type="text" class="form-control" name="surname"  id="surname" >
                </div>
              </div>
                
                </div>
              <!-- /.box-body -->
              <div class="box-footer">
                <button type="submit" id="myBtn" target="myModal">Search Co-authors</button>
              </div>

              <!-- /.box-footer -->
            </form>
</div>  <?php
session_start();
$user_id =  $_SESSION['user_id'];

  if($user_id === 0){ ?>
      <a href= "admin_menu.php" class="button">Go Menu</a>
  <?php }else{?>
    <a href= "user_menu.php" class="button">Go Menu</a>
<?php }?>
