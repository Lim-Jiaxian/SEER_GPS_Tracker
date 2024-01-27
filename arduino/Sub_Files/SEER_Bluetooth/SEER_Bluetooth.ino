//Bluetooth serial library
#include <BluetoothSerial.h>

//Create bluetooth serial object
BluetoothSerial SerialBT;

//Create a string to store the bluetooth mac address
String bluetoothMacAddress = "";

void setup() {
  //Begin serial at 9600 buadrate for serial monitor
  Serial.begin(9600);
  //Set bluetooth name for ESP32
  SerialBT.begin("SEER Tracker ESP32 Bluetooth"); 
  //Set the callback function to handle incoming Bluetooth connections
  SerialBT.register_callback(handleBluetoothConnection);
}

void loop() {
  //Checks the bluetooth mac address length
  if(bluetoothMacAddress.length() > 1)
  {
    Serial.println("Device Bluetooth Mac Address: " + bluetoothMacAddress);
  }else
  {
    Serial.println("Mobile phone is not connect to SEER GPS Tracker via bluetooth.");
  }
}

//Handles events related to bluetooth connections
//esp_spp_cb_event_t parameter specifies the Bluetooth event
//esp_spp_cb_param_t parameter holds information about the event
void handleBluetoothConnection(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  //Checks if bluetooth connection is successful
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Bluetooth device connected!");

    //Obtain the bluetooth device mac address and store it to a 8-bit integer data type
    uint8_t* deviceMacAddress = param->srv_open.rem_bda;
    Serial.print("Bluetooth MAC Address: ");

    //For loop to check each mac address byte (6)
    for (int i = 0; i < 6; i++) {
      //Convert each byte to a two-digit hexadecimal string, concatenate and add to string
      bluetoothMacAddress += String(deviceMacAddress[i], HEX);
      //Add a colon between bytes (except for the last hexadecimal)
      if (i < 5) bluetoothMacAddress += ":";
    }

    //Print the MAC address
    // Serial.println(bluetoothMacAddress);
  }
}





