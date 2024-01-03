<?php
$serverhostname = "localhost";
$username = "root";
$password = "";
$database = "geotrackdb";
//Establish connection to MySQL database
$mysqliconn = mysql_connect($host, $user, $password, $database);
//Check connection
if ($mysqli->connect_errno) {
    die("Failed to connect to MySQL: " . mysqli_connect_error());
    exit();
}
echo "Connected successfully";
?>