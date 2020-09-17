<!DOCTYPE html>
<?php
session_start();
$_SESSION['user_id'] = 0; 
            ?>
<html>
        <body>
                        <li class="treeview">
                          <a class="menu-option" ><i class="fa fa-link" ></i> <span>Database Changes</span>
                            <span class="pull-right-container">
                                    <i class="fa fa-angle-left pull-right"></i>
                                  </span>
                          </a>
                
                          <ul class="treeview-menu">
                            <li class="active"><a href="add_author.php">Add Author</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="edit_author.php">Edit Author</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="delete_author.php">Delete Author</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="add_paper.php">Add Paper</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="edit_paper_view.php">Edit Paper</a></li>
                          </ul>       
                          <ul class="treeview-menu">
                            <li class="active"><a href="delete_paper_view.php">Delete Paper</a></li>
                          </ul>                          
                          <ul class="treeview-menu">
                            <li class="active"><a href="add_topic.php">Add Topic</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="edit_topic.php">Edit Topic</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="delete_topic.php">Delete Topic</a></li>
                          </ul>
                        </li>
                        <li class="treeview">
                          <a class="menu-option" ><i class="fa fa-link" ></i> <span>Views</span>
                            <span class="pull-right-container">
                                    <i class="fa fa-angle-left pull-right"></i>
                                  </span>
                          </a>
                          <ul class="treeview-menu">
                            <li class="active"><a href="view_authors.php">View Authors</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="view_papers.php">View Papers</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="view_topics.php">View Topics</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="view_papers_of_author.php">View All Papers of an Author</a></li>
                          </ul>                          
                          <ul class="treeview-menu">
                            <li class="active"><a href="view_sota_results_all.php">View All Sota Results by Topic </a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="view_sota_paper_topic.php">View Papers on a Specific Topic</a></li>
                          </ul>                          
                          <ul class="treeview-menu">
                            <li class="active"><a href="rank_authors.php">Rank Authors by Their Number of SOTA Results</a></li>
                          </ul>                          
                          <ul class="treeview-menu">
                            <li class="active"><a href="search_keyword.php">Search a Keyword in Title/Abstract and View</a></li>
                          </ul>
                          <ul class="treeview-menu">
                            <li class="active"><a href="view_coauthor_search.php">View Co-authors of an Author</a></li>
                          </ul>

                        </li>
                      </ul>
                      <!-- /.sidebar-menu -->
                    </section>
                    <!-- /.sidebar -->
                  </aside>
                  
                  <a href= "check_admin_user.php" class="button">Be User</a>

    </body>
</html>