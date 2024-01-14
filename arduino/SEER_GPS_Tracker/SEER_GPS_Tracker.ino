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


//Wifi network credentials
const char* ssid = "K'11";
const char* password = "a1357b24689";

//Declaring the apache server address to php file for posting data
const char* apacheServer = "http://192.168.43.190/seer/php/GpsPostData.php";
//const char* apacheServer = "http://192.168.43.93/seer/php/GpsPostData.php";

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

void setup()
{
  //Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  //Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  //Initialize I2C communication via SDA & SCL pins for batometric sensor
  Wire.begin(sda, scl);

  //Connect to wifi network
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED) //Not connected to wifi
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("ESP32 NodeMCU IP: ");
  Serial.println(WiFi.localIP());
}

//Display captured records of Altitude, Latitude and Longtitude
void displayInfo(TinyGPSDate& dtDate, TinyGPSTime& dtTime, double& baroAlt, double& gpsLat, double& gpsLng, int& estiFlr) //Variable references in the function
{
  if(ms5611.isConnected() == true) //Check if MS5611 Barometric sensor is connected
  {
    //Obtain pressure readings
    long bmpPressure = ms5611.getPressure();

    //International Barometric formula to calculate altitude 
    //h = 44330 * [1 - (p / p0)^(1/5.255)] 
    //h = altitude 
    //p = pressure in pascals (Pa)
    //p0 = reference pressure at sea level

    //Calculate altitude
    double bmpAltitude = 44330.0 * (1 - pow(bmpPressure/ referencePressure, 1 / 5.255)); 

    //Calculate estimated floor level from dividing altitude by 3. Assuming each floor has a difference of 3 meters.
    estiFlr = round((bmpAltitude / 3));
    if(estiFlr < 0) //Checks if altitude divide by 3 falls below 0
    {
      estiFlr = 0; //Change the estimated floor to 0 which is ground level
    }
    Serial.print("Current estimated floor: ");
    Serial.print(estiFlr);
    Serial.println(" level");
    Serial.print("Altitude: ");
    Serial.print(bmpAltitude);
    Serial.println(" meters");
    baroAlt = bmpAltitude; //Update variable to value of altitude
  }
  else
  {
    Serial.println("Barometric Altitude: Not Available");
  }

  if (gps.location.isValid()) //If location is valid
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    gpsLat = gps.location.lat(); //Update variable to the value of latitude
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6); 
    gpsLng = gps.location.lng(); //Update variable to the value of longitude
  }
  else
  {
    Serial.println("Location: Not Available");
  }

  
  
  Serial.print("Date: ");
  if (gps.date.isValid()) //If date is valid
  {
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.println(gps.date.year());
    dtDate = gps.date; //Variable to store date from gps
  }
  else
  {
    Serial.println("Date: Not Available");
  }

  //Print time of captured GPS coordinates
  Serial.print("Time: ");
  dtTime = gps.time; //Variable to store time from gps
  if (gps.time.isValid())
  {
    if ((gps.time.hour() + 8) >= 24 ) //Checks if hour exceeds 24 when being represented in 24 hour format
    {
      Serial.print((gps.time.hour() + 8) - 24); //Deduct 24 to remain in 24 hour format
    }
    else
    {
      Serial.print(gps.time.hour() + 8); //Add 8 to remain in 24 hour format
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
  //Declare variables
  TinyGPSDate dtDate; //dtDate to store date from TinyGPSDate
  TinyGPSTime dtTime; //dtTime to store time from TinyGPSTime
  double baroAlt; //Barometric Altitude
  double gpsLat; //GPS Latitude
  double gpsLng; //GPS Longitude
  int estiFlr; //Estimated floor from altitude
  int hourtime = 0;


  //Barometric Pressure Sensor
  //Initialize the MS5611 sensor module
  ms5611.begin();
  if(ms5611.isConnected() == true) //Check if MS5611 Barometric sensor is connected
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
      displayInfo(dtDate, dtTime, baroAlt, gpsLat, gpsLng, estiFlr);
      String outputJSON; //String to store JSON
      DynamicJsonDocument doc(200); //Serialization of data in JSON
      doc["secguardid"] = 1; //Set security guard id
      if (dtDate.isValid() && dtTime.isValid())
      {
        if((dtTime.hour() + 8) >= 24) //Checks if hour exceeds 24 when being represented in 24 hour format
        {
          hourtime = ((dtTime.hour() + 8) - 24); //Deduct 24 to remain in 24 hour format
        }
        else
        {
          hourtime = dtTime.hour() + 8; //Add 8 to remain in 24 hour format
        }
        char dt[30]; //A character array to store date time
        //Format the date and time string and store it in the dt array
        sprintf(dt, "%02u-%02u-%04u %02u:%02u:%02u", dtDate.year(), dtDate.month(), dtDate.day(), hourtime, dtTime.minute(), dtTime.second()); 
        doc["recordtime"] = String(dt); //Assign the formatted date and time string to a JSON document with the key "recordtime"
      }
      if (gps.location.isValid())
      {
        doc["altitude"] = baroAlt;
        doc["latitude"] = gpsLat;
        doc["longitude"] = gpsLng;
        doc["floor"] = estiFlr;
      }
      serializeJson(doc, outputJSON); //Serialize the JSON into a string, storing it in outputJSON
      Serial.println(outputJSON); //Print JSON

      //Check for wifi connection before sending data out as JSON
      if(WiFi.status() == WL_CONNECTED)
      {
        WiFiClient client; //Initialise variable client from wificlient lib
        HTTPClient http; //Initialise variable http from httpclient lib
        http.begin(client, apacheServer); //Initiate a http request with the apache server
        http.addHeader("Content-Type", "application/json"); //Declare json format
        int responseCodeHttp = http.POST(outputJSON); //Variable to obtain http response code
        if(responseCodeHttp >= 200 && responseCodeHttp <= 299) //Checks if the http response code is successful from 200 - 299
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
      
  if (millis() > 5000 && gps.charsProcessed() < 10)//If 5000 milliseconds has passed without characters being processed from the gps, it indicates that no GPS is detected.
  {
    Serial.println("No GPS detected");
  }
}