<?php
//Checks if it is a HTTP request method is POST
if ($_SERVER["REQUEST_METHOD"] == 'POST')
{
    //Include database connection php file
    include('GeoDBConnection.php');

    //Get JSON data from HTTP request & decode json data into php array
    $json = file_get_contents('php://input');
    $gpsdata = json_decode($json, true);

    // Prepare SQL statement for inserting data into database, followed by binding the parameters to the SQL statement and executing the sql statement
    $sqlstatement = $mysqliconn->prepare("INSERT INTO Records (SecGuardID, RecordTime, Altitude, Latitude, Longitude) VALUES (?, ?, ?, ?, ?);"); 
    $sqlstatement->bind_param("isddd", $gpsdata['secguardid'], $gpsdata['recordtime'], $gpsdata['altitude'], $gpsdata['latitude'], $gpsdata['longitude']); 
    $sqlstatement->execute(); 
}
else //If HTTP request method is not POST
{
    //Display error message and exit script
    die("ERROR: Request method not POST"); 
}
?>
