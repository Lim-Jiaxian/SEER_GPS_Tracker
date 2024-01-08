<?php
$serverhostname = "localhost";
$username = "root";
$password = "";
$database = "geotrackdb";
//Establish connection to MySQL database
$mysqliconn = mysqli_connect($serverhostname, $username, $password, $database);
//Check connection
if ($mysqliconn->connect_errno) {
    die("Failed to connect to MySQL: " . mysqli_connect_error());
    exit();
}
echo "Connected successfully";

$json = file_get_contents('php://input');
$gpsdata = json_decode($json, true);

$sqlstatement = $mysqliconn->prepare("INSERT INTO Records (SecGuardID, RecordTime, Altitude, Latitude, Longitude) VALUES (?, ?, ?, ?, ?);");
$sqlstatement->bind_param("isddd", $gpsdata['secguardid'], $gpsdata['recordtime'], $gpsdata['altitude'], $gpsdata['latitude'], $gpsdata['longitude']);
$sqlstatement->execute();

?>
