# SEER GPS Tracker Information

**What is SEER GPS Tracker?** It is a portable device designed to track the movements and pathways of a security guard to identify potential blind spots or areas that may have been overlooked during surveillance.

**How does it work?** It utilizes a Neo 6m GPS Tracker to capture geo locations of latitude and longitude as well as a Barometric Pressure Sensor to calculate altitude from atmospheric pressure information.

By utilizing the SEER GPS Tracker, security guards can conveniently carry the device with them and have their movements and routes recorded. The Neo 6m GPS Tracker captures the geographical coordinates of latitude and longitude. While the Barometric Pressure Sensor calculates the altitude to further enhance the tracking capabilities. 

In the event of a lost GPS signal, which typically occurs when the security guard enters a building, the SEER GPS Tracker will utilize the most recent latitude and longitude coordinates recorded. The altitude will be used to calculate the estimated floor / level, the security guard is currently located.

However, the altitude readings is subjected to change based on external factors. Barometric pressure sensors rely on measuring atmospheric air pressure to determine altitude. Therefore, environmental conditions that affect air pressure can affect or hinder the accuracy of altitude readings. For example, changes in weather patterns, such as the presence of heavy fog, rain, or strong winds, can cause fluctuations in air pressure and result in inaccurate altitude measurements.

<br>

### Equipment Pre-Requisites & Technologies Implemented 
- Equipment
  - ESP32 NodeMCU (Development Kit)
  - Neo 6m (GPS Tracker)
  - GY-63 MS5611 (Barometric Pressure Sensor)
  - Jumper wires for connection
- Technologies
  - XAMPP Server
  - MySQL Database (Store GEO Locations)
  - Apache Webserver (Host SEER Webpage)
- Software
  - Arduino IDE, C++ code with multiple imported library functions for data collection of GEO locations (Uploaded to ESP32 NodeMCU)
  -  Python libraries such as Matplotlib and Cartopy to plot data visulisation of captured GEO locations

<br>

### Logical Diagram of SEER Tracker

![Logical Diagram](https://github.com/Lim-Jiaxian/SEER_GPS_Tracker/blob/main/Images/Logical_Diagram.PNG)

#### Summary of logical diagram

The ESP32 NodeMCU with its built-in Wi-Fi and Bluetooth capabilities can effectively ensure the security guard's identity and communicate with Apache web server to send data collected from the GPS Tracker and Barometric Pressure Sensor. In an example scenario, the ESP32 NodeMCU will be used to collect the GPS coordinates, that is transmitted to a MySQL database via a HTTP POST request to the PHP script on the Webpage. PHP, a scripting language, will handle the server-side processing of receiving the data from the ESP32, and storing it in MySQL database. Subsequently, a Python script with multiple libraries such as matplotlib and cartopy will be used to plot out the entire route and pathway of the security guard to visualise for areas of improvements.

### Individual Contributions
Cayden Xie: Pyscript & Web GUI
Lim Jia Xian: ESP32 NodeMCU
Jerel Sim: Python (Matplotlib & Cartopy)

