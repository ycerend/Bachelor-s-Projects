<?php
/*
 * Tutorial: PHP Login Registration system
 *
 * Page index.php
 * */
 
// Start Session
session_start(); 
$_SESSION['user_id'] = -1; 
?>

<html>
    <head>
        <title>
          Project Management System Home Page
        </title>
    </head>
    <body>
        Select type of user:

        <ol>
          <li> <a href="admin_menu.php">Admin </a></li>
          <li> <a href="user_menu.php">User</a></li>
        </ol>
    </body>
</html>