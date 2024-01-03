//Import Libraries

//TinyGPSPlus 
#include <TinyGPS++.h>
#include <TinyGPSPlus.h>

//ArduinoJson 
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

//EspSoftwareSerial 
#include <SoftwareSerial.h>

//Wifi network connection
#include <WiFi.h>

//Wifi network credentials
const char* ssid = "K'11";
const char* password = "a1357b24689";

//Initialise apache server ip
//const char* apacheServer = "http: 192.168.43.190/seer_api/php_file"

//Two Arduino pins for software serial
int RXPin = 16;
int TXPin = 17;

// GPS Buadrate
int GPSBaud = 9600;

//Create a TinyGPS++ object
TinyGPSPlus gps;
//Create a software serial port for gpsSerial with the Arduino pins
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  //Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  //Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
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

//Display GPS captured records of Altitude, Latitude and Longtitude
void displayInfo(TinyGPSDate& dtDate, TinyGPSTime& dtTime, double& gpsAlt, double& gpsLat, double& gpsLng)
{
  if (gps.location.isValid()) //If location is valid
  {
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
    gpsAlt = gps.altitude.meters();
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    gpsLat = gps.location.lat();
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6); 
    gpsLng = gps.location.lng();
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
    Serial.println("Not Available");
  }

  //Print time of captured GPS coordinates
  Serial.print("Time: ");
  dtTime = gps.time;
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F(""));
    Serial.print(gps.time.hour() + 8);
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

//Main loop for ESP32
void loop()
{
  //Declare variables
  TinyGPSDate dtDate;
  TinyGPSTime dtTime;
  double gpsAlt;
  double gpsLng;
  double gpsLat;
  //Displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      displayInfo(dtDate, dtTime, gpsAlt, gpsLat, gpsLng);
      String outputJSON; //String to store JSON
      DynamicJsonDocument doc(200); //Serialization of data in JSON
      doc["secguardid"] = 1; //Set security guard id
      if (gps.location.isValid())
      {
        doc["altitude"] = gpsAlt;
        doc["longitude"] = gpsLng;
        doc["latitude"] = gpsLat;
      }
      if (dtDate.isValid() && dtTime.isValid())
      {
        char dt[30]; //A character array to store date time
        //Format the date and time string and store it in the dt array
        sprintf(dt, "%02u-%02u-%04u %02u:%02u:%02u", dtDate.day(), dtDate.month(), dtDate.year(), dtTime.hour() + 8, dtTime.minute(), dtTime.second()); 
        doc["recordtime"] = String(dt); //Assign the formatted date and time string to a JSON document with the key "recordtime"
      }
      serializeJson(doc, outputJSON); //Serialize the JSON into a string, storing it in outputJSON
      Serial.println(outputJSON); //Print JSON
      //Check for wifi connection before sending data out as JSON
      /*
      if(WiFi.status() == WL_CONNECTED)
      {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, apacheServer);
        http.addHeader("Content-Type", "application/json");
        int responseCodeHttp = http.post(outputJSON);
        if(responseCodeHttp >= 200 && <= 299)
        {
          Serial.println("JSON output delivered to apache server.")
        }
      }*/
    }
  }
      
  if (millis() > 5000 && gps.charsProcessed() < 10)//If 5000 milliseconds has passed without characters being processed from the gps, it indicates that no GPS is detected.
  {
    Serial.println("No GPS detected");
    //while(true);//Enter a loop
  }
}