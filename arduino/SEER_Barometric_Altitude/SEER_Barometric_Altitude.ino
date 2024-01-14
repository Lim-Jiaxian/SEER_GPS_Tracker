#include <Wire.h>
#include <MS5611.h>

//Initialize MS5611 Barometric object with I2C (0x77) address
MS5611 ms5611(0x77);

//Reference pressure at sea level
double referencePressure = 1013.25;

void setup() 
{
  //Initialize SDA & SCL pins
  int sda = 21;
  int scl = 22;
  Serial.begin(9600);
  //Initialize I2C communication via SDA & SCL pins
  Wire.begin(sda, scl);
}


void loop()
{
  //Initialize the MS5611 sensor module
  ms5611.begin();
  if(ms5611.isConnected() == true) //Check if MS5611 Barometric sensor is connected
  {
    //Reads the data from the MS5611 sensor module
    ms5611.read();
    //Read temperature & Pressure
    double bmpTemperature = ms5611.getTemperature();
    long bmpPressure = ms5611.getPressure();
    Serial.println("--");
    Serial.print("Current Temperature: ");
    Serial.print(bmpTemperature);
    Serial.println(" *C");
    Serial.print("Current Pressure: ");
    Serial.print(bmpPressure);
    Serial.println(" Pa");

    //International Barometric formula to calculate altitude 
    //h = 44330 * [1 - (p / p0)^(1/5.255)] 
    //h = altitude 
    //p = pressure in pascals (Pa)
    //p0 = reference pressure at sea level

    //Calculate altitude
    float bmpAltitude = 44330.0 * (1 - pow(bmpPressure/ referencePressure, 1 / 5.255)); 
    Serial.print("Current Altitude: ");
    Serial.print(bmpAltitude);
    Serial.println(" meters");

    //Calculate estimated floor level from dividing altitude by 3. Assuming each floor has a difference of 3 meters.
    int estFloor = round((bmpAltitude / 3));
    Serial.print("Current estimated floor: ");
    Serial.print(estFloor);
    Serial.println(" level");

    delay(1000);

  }
  else
  {
    Serial.println("MS5611 not connected.");
    delay(1000);
  }
}