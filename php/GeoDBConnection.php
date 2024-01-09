<?php
//Database connection variables
$serverhostname = "localhost"; 
$username = "root";
$password = "";
$database = "geotrackdb";

//Establish connection to MySQL database
$mysqliconn = mysqli_connect($serverhostname, $username, $password, $database);
//Check connection
if ($mysqliconn->connect_errno) {
    //Display error message and exit script
    die("Failed to connect to MySQL: " . mysqli_connect_error()); 
    exit();
}

//Show success message if connection is successful
echo "Connected successfully";
?>
