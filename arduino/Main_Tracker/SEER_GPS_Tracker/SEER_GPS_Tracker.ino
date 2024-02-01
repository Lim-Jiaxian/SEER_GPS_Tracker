//Import Libraries

//HTTPClient
#include <HTTPClient.h>

//TinyGPSPlus 
#include <TinyGPSPlus.h>

//ArduinoJson 
#include <ArduinoJson.h>

//EspSoftwareSerial 
#include <SoftwareSerial.h>

//Wifi 
#include <WiFi.h>

//Wire for I2C with SDA & SCL Pins
#include <Wire.h>

//Barometric Sensor 
#include <MS5611.h>

//BluetoothSerial
#include <BluetoothSerial.h>

//Wifi network credentials
const char* ssid = "K'11";
const char* password = "a1357b24689";

//Declaring the apache server address to php file for posting data
const char* apacheServer = "http://192.168.43.93/seer/GpsPostData.php";
//const char* apacheServer = "http://192.168.43.93/seer/php/GpsPostData.php";
//const char* apacheServer = "http://192.168.43.190/GpsPostData.php";

//Two Arduino pins for software serial
int RXPin = 16;
int TXPin = 17;

//Initialize SDA & SCL pins
int sda = 21;
int scl = 22;

// GPS Buadrate
int GPSBaud = 9600;

//Initialize MS5611 Barometric object with I2C (0x77) address
MS5611 ms5611(0x77);

//Create a TinyGPS++ object
TinyGPSPlus gps;

//Create a software serial port for gpsSerial with the Arduino pins
SoftwareSerial gpsSerial(RXPin, TXPin);

//Reference pressure at sea level
double referencePressure = 1013.25;
//Reference pressure at sea level 1013.25 hPa, 101325 Pa

//Create bluetooth serial object
BluetoothSerial SerialBT;

//Create a string to store the bluetooth mac address
String bluetoothMacAddress = "";



void setup()
{
  //Start the arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  //Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  //Initialize I2C communication via SDA & SCL pins for batometric sensor
  Wire.begin(sda, scl);

  //Set bluetooth name for ESP32
  SerialBT.begin("SEER Tracker ESP32 Bluetooth"); 
  //Set the callback function to handle incoming Bluetooth connections
  SerialBT.register_callback(handleBluetoothConnection);

  //Connect to wifi network
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  //while loop to check wifi connection
  while(WiFi.status() != WL_CONNECTED) //Not connected to wifi
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("ESP32 NodeMCU IP: ");
  Serial.println(WiFi.localIP());
}

//Function to handle bluetooth connections events
//esp_spp_cb_event_t parameter specifies the Bluetooth event
//esp_spp_cb_param_t parameter holds information about the event
void handleBluetoothConnection(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  //Checks if bluetooth connection is successful
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    //Set bluetooth mac address to empty (Main program will loop to check bluetooth connection)
    bluetoothMacAddress = "";
    Serial.println("Bluetooth device connected!");
    //Obtain the bluetooth device mac address and store it to a 8-bit integer data type
    uint8_t* deviceMacAddress = param->srv_open.rem_bda;
    //Serial.print("Bluetooth MAC Address: ");

    //For loop to check each mac address byte (6)
    for (int i = 0; i < 6; i++) {
      //Convert each byte to a two-digit hexadecimal string, concatenate and add to string
      bluetoothMacAddress += String(deviceMacAddress[i], HEX);
      //Add a colon between bytes (except for the last hexadecimal)
      if (i < 5) {
        bluetoothMacAddress += ":";
      }
    }
  }
  else//bluetooth connection not successful
  {
    //Set bluetooth mac address to empty
    bluetoothMacAddress = "";
  }
}

//Display captured records of Altitude, Latitude and Longtitude
void displayInfo(TinyGPSDate& dtDate, TinyGPSTime& dtTime, double& baroAlt, double& gpsLat, double& gpsLng, int& estiFlr) //Variable references in the function
{
  //Check if MS5611 Barometric sensor is connected
  if(ms5611.isConnected() == true) 
  {
    //Obtain pressure readings
    long bmpPressure = ms5611.getPressure();

    //International Standard Atmosphere (ISA) / Barometric formula to calculate altitude 
    //h = 44330 * [1 - (p / p0)^(1/5.255)] 
    //h = altitude 
    //p = pressure in pascals (Pa)
    //p0 = reference pressure at sea level

    //Calculate barometric altitude using ISA / Barometric formula
    baroAlt = 44330.0 * (1 - pow(bmpPressure/ referencePressure, 1 / 5.255)); 

    //Average elevation / altitude above sea level around Ngee Ann Polytechnic at ground level from GEO survey
    double elvaAltitude = 32.10; //If altitude readings are slightly off due to weather conditions, use altitude reading of 17.
    //Elevation / altitude above sea level for blk 31 & 27 in Ngee Ann Polytechnic
    //double elvaAltitude = 36.50;

    //Calculate estimated floor level
    //Current altitude difference = Barometric altitude - Elevation altitude
    //Round the floor level to a single digit after dividing the difference in altitude by 2.5. Assuming each floor has a difference of 2.5 meters.
    estiFlr = round(((baroAlt - elvaAltitude) / 2.5));
    //Calculate estimated floor level from dividing altitude by 4. Assuming each floor has a difference of 4 meters.
    //estiFlr = round(((baroAlt - elvaAltitude) / 4));
    //Checks if altitude divided by floor difference (meters) falls below 0
    if(estiFlr < 0) 
    {
      //Change the estimated floor to 1 which is ground level
      estiFlr = 1; 
    }
    Serial.print("Device Bluetooth Mac Address: ");
    Serial.println(bluetoothMacAddress);
    Serial.print("Current estimated floor: ");
    Serial.print(estiFlr);
    Serial.println(" level");
    Serial.print("Altitude: ");
    Serial.print(baroAlt);
    Serial.println(" meters");
  }
  else
  {
    Serial.println("Barometric Altitude: Not Available");
  }

  //Check if GPS location is valid
  if (gps.location.isValid()) 
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    //Update variable to the value of latitude
    gpsLat = gps.location.lat(); 
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6); 
    //Update variable to the value of longitude
    gpsLng = gps.location.lng(); 
  }
  else
  {
    Serial.println("Location: Not Available");
  }

  Serial.print("Date: ");
  //Check if date is valid
  if (gps.date.isValid())
  {
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.println(gps.date.year());
    //Variable to store date from gps
    dtDate = gps.date; 
  }
  else
  {
    Serial.println("Date: Not Available");
  }

  //Print time of captured GPS coordinates
  Serial.print("Time: ");
  //Variable to store time from gps
  dtTime = gps.time; 

  //Check if GPS time is valid
  if (gps.time.isValid())
  {
    //Checks if hour exceeds 24 when being represented in 24 hour format
    if ((gps.time.hour() + 8) >= 24 ) 
    {
      //Deduct 24 to remain in 24 hour format
      Serial.print((gps.time.hour() + 8) - 24); 
    }
    else
    {
      //Add 8 to remain in 24 hour format
      Serial.print(gps.time.hour() + 8); 
    }
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.print(gps.time.second());
    Serial.print(".");
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Time: Not Available");
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

//Main loop for ESP32
void loop()
{
  //SerialBT.register_callback(handleBluetoothConnection);
  //Initialise variables
  TinyGPSDate dtDate; //dtDate to store date from TinyGPSDate
  TinyGPSTime dtTime; //dtTime to store time from TinyGPSTime
  double baroAlt; //Barometric Altitude
  double gpsLat; //GPS Latitude
  double gpsLng; //GPS Longitude
  int estiFlr; //Estimated floor from altitude
  int hourtime = 0; //Hourtime

  //while loop to check wifi connection
  while(WiFi.status() != WL_CONNECTED) //Not connected to wifi
  {
    Serial.println("Wifi connection not established with SEER Tracker");
    delay(100);
  }

  //Set the callback function to handle incoming Bluetooth connections
  SerialBT.register_callback(handleBluetoothConnection);

  //while loop to check bluetooth connection
  while(bluetoothMacAddress.length() == 0)
  {
    Serial.println("Bluetooth connection not established with SEER Tracker");
    delay(100);
  }


  //Barometric Pressure Sensor
  //Initialize the MS5611 sensor module
  ms5611.begin();
  //Check if MS5611 Barometric sensor is connected
  if(ms5611.isConnected() == true) 
  {
    //Reads the data from the MS5611 sensor module
    ms5611.read();
  }
  else
  {
    Serial.println("MS5611 not connected.");
    delay(1000);
  }


  //GPS Tracker
  //Displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      //Function to display information & update GPS values
      displayInfo(dtDate, dtTime, baroAlt, gpsLat, gpsLng, estiFlr);
      //String to store JSON
      String outputJSON; 
      //Create JSON doc of 200 bytes
      DynamicJsonDocument doc(200); 
      //Set security guard id 
      doc["secguardid"] = 1;
      //Set security guard's bluetooth mac address from mobile
      doc["btmacaddress"] = bluetoothMacAddress;

      //Check if date & time is valid
      if (dtDate.isValid() && dtTime.isValid())
      {
        //Checks if hour exceeds 24 when being represented in 24 hour format
        if((dtTime.hour() + 8) >= 24) 
        {
          //Deduct 24 to remain in 24 hour format
          hourtime = ((dtTime.hour() + 8) - 24); 
        }
        else
        {
          //Add 8 to remain in 24 hour format
          hourtime = dtTime.hour() + 8; 
        }
        //A character array to store date time
        char dt[30]; 
        //Format the date and time string and store it in the dt array
        sprintf(dt, "%02u-%02u-%04u %02u:%02u:%02u", dtDate.year(), dtDate.month(), dtDate.day(), hourtime, dtTime.minute(), dtTime.second()); 
        //Assign the formatted date and time string to a JSON document with the key "recordtime"
        doc["recordtime"] = String(dt); 
      }
      //Checks if GPS location is valid
      if (gps.location.isValid())
      {
        //Assign the Geolocation value to JSON document keys
        doc["altitude"] = baroAlt;
        doc["latitude"] = gpsLat;
        doc["longitude"] = gpsLng;
        doc["floor"] = estiFlr;
      }
      //Serialize the JSON into a string, storing it in outputJSON
      serializeJson(doc, outputJSON); 
      //Print JSON
      Serial.println(outputJSON); 

      //Check for wifi connection before sending data out as JSON
      if(WiFi.status() == WL_CONNECTED)
      {
        //Initialise wificlient object
        WiFiClient client; 
        //Initialise httpclient object
        HTTPClient http; 
        //Begin http request with the apache server
        http.begin(client, apacheServer); 
        //Declare JSON format
        http.addHeader("Content-Type", "application/json"); 
        //Variable to obtain http response code
        int responseCodeHttp = http.POST(outputJSON); 
        //Checks if the http response code is successful from 200 - 299
        if(responseCodeHttp >= 200 && responseCodeHttp <= 299) 
        {
          Serial.print("JSON output delivered to apache server. ");
        }else
        {
          Serial.print("JSON output NOT delivered to apache server. ");
        }
        Serial.println("HTTP Reponse Code: " + String(responseCodeHttp));
      }
    }
  }

  //Check if 5000 milliseconds has passed without characters being processed from the gps, indicating that no GPS is detected.
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
  }
}