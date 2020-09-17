
<div>
            <!-- /.box-header -->
            <!-- form start -->
            <form class="form-horizontal" id="submit_form" action="list_search_keyword.php">
            
            <div class="box-body">
                <div class="form-group">
                
                  <label for="keyword" name="keyword" class="col-sm-2 control-label">Keyword</label>

                  <div class="col-sm-10">
                    <input type="text" class="form-control" name="keyword"  id="keyword" >
                  </div>
                </div>
                
                </div>
              <!-- /.box-body -->
              <div class="box-footer">
                <button type="submit" id="myBtn" target="myModal">Search</button>
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
