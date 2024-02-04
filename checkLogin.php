<?php
// Detect the current session
session_start();

//Reading inputs entered in previous page
$email= $_POST["email"];
$pwd = $_POST["password"];

// Include the PHP file that establishes database connection handle: $conn
include_once("GeoDBConnection.php");

$qry = "SELECT * FROM adminlogin WHERE Email LIKE '%$email%' AND Password LIKE '%$pwd%'";
$result = $conn->query($qry);

// To Do 1 (Practical 2): Validate login credentials with database
if ($result->num_rows > 0) {
    //Save user's info in session variables
    while ($row = $result->fetch_array())
    $_SESSION["LoginID"] = $row["LoginID"];

    //Redirect to home page
    header("Location: index.html");
    exit;
}
else {
    echo "<h3 style='color:red'> Invalid Login Credentials</h3>";
}
$conn->close();

?>
