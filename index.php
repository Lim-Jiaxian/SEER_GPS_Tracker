<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Project SEER</title>
    <link rel="stylesheet" href="css/styles.css">
    <script src="javascript/IndexJavascript.js"></script>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Poppins:ital,wght@0,300;0,400;0,600;0,700;1,200&display=swap" rel="stylesheet">
</head>


<?php

session_start();

include_once("GeoDBConnection.php");

$query = "SELECT * FROM secguard";
$result = $conn->query($query);

?>
    

<body>
    <section class="header">
        <nav>
            <a href="index.php"><img src="Images/SEER (Name Ver).png"></a>
            <div class="nav-links">
                <ul>
                    <li><a href="index.php">HOME</a></li>
                    <li><a href="about.php">ABOUT</a></li>
                    <li><a href="">SERVICES</a></li>
                    <li><a href="">CONTACT</a></li> 
                    <li><a href="login.html">LOGIN</a></li> 
                </ul>
            </div>
        </nav>
      <br>

    <div class="home-post" style="background-color:whitesmoke;">
        <h2>Pathway Tracker</h2><br>
        <!--<img src="" height="250" width="600" class="center">
        <img src="" class="center">-->
        <p>
            -  Security Guard's pathway  -
        </p><br>
        

        
        <p>
        <img src="Images/Individual_Plots/plot_SecGuardID_1.png" alt="Image">
          <img src="Images/Individual_Plots/plot_SecGuardID_2.png" alt="Image">
          <!-- <img src="Images\plot_SecGuardID_1.png" alt="Image" width="100%" height="80%">
          <img src="Images\plot_SecGuardID_2.png" alt="Image" width="100%" height="80%"> -->
        </p>
        <button onclick="runSeer()"> Run Seer </button>
        <br><br>
    </div>


    <div class="footer">
        <p>Contact Us: +65 12345678</p>
    </div>
</body>

</html>   