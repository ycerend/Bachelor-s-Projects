<?php
$dsn = "mysql:dbname=cmpe321_project3"; 
$username_database = "root"; 
$password_database = ""; 
try{
$conn = new PDO( $dsn, $username_database, $password_database );  
$conn->setAttribute(PDO::ATTR_ERRMODE,PDO::ERRMODE_EXCEPTION);
}
catch(PDOException $pd){
echo $pd->getMessage();
}
?>