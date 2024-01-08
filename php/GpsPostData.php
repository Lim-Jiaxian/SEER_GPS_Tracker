<?php
// Obtain connection to MySQL database
require_once('dbConnection.php');
$json = file_get_contents('php://input');
$gpsdata = json_decode($json, true);

$sqlstatement = $mysqliconn->prepare("INSERT INTO Records (SecGuardID, RecordTime, Altitude, Latitude, Longitude) VALUES (?, ?, ?, ?, ?);");
$sqlstatement->bind_param("isddd", $gpsdata['secguardid'], $gpsdata['recordtime'], $gpsdata['altitude'], $gpsdata['latitude'], $gpsdata['longitude']);
$sqlstatement->execute();


if ($_SERVER["REQUEST_METHOD"] !== 'POST')
{
    die("ERROR: Request method not POST");
}
?>