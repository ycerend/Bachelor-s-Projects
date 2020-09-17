<!DOCTYPE html>
<?php
session_start();
$_SESSION['user_id'] = 1; 
            ?>
<html>
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

                  
                  <a href= "check_admin_user.php" class="button">Be Admin</a>
</html>