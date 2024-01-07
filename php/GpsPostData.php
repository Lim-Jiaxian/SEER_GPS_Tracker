<?php
// Obtain connection to MySQL database
require_once('dbConnection.php');
if ($_SERVER["REQUEST_METHOD"] !== 'POST')
{
    die("ERROR: Request method not POST");
}
?>