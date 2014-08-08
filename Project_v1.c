// This #include statement was automatically added by the Spark IDE.
#include "sd-card-library/sd-card-library.h"

#include "application.h"
#include "spark_disable_wlan.h"
#include "spark_disable_cloud.h"
#include "sd-card-library/sd-card-library.h"
WiFi_Status_TypeDef last_wifi_status;


TCPServer server = TCPServer(23);
TCPClient client;
int analogPin = A0;
int analogPin2 = A1;
int value = 0;
File myFile;

const uint8_t chipSelect = A2;    // Also used for HARDWARE SPI setup
const uint8_t mosiPin = A5;
const uint8_t misoPin = A4;
const uint8_t clockPin = A3;

void setup()
{

  last_wifi_status = WiFi.status();
  WiFi.on();
  server.begin();
  Serial.begin(9600);
  server.print("Initializing SD card...");  
  
  // Initialize SOFTWARE SPI
  if (!SD.begin(mosiPin, misoPin, clockPin, chipSelect)) {
    server.println("initialization failed!");
    return;
  }

  server.println("initialization done.");
}

void loop()
{
  Serial.println(Network.localIP());
  if (client.connected()) {
    //Serial.println(micros());
    value = analogRead(analogPin) - analogRead(analogPin2);
    myFile = SD.open("ECG.txt", FILE_WRITE);
    if (myFile) {
      myFile.println(value);
      myFile.print(";");
      server.print(value);
      server.print(";");
      // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      server.println("error opening test.txt");
    }
  } else {
   // digitalWrite(D7,LOW);
    // if no client is yet connected, check for a new connection
    //delay(1000);
    client = server.available();
    //digitalWrite(D7,HIGH);
   // delay(1000);
    
  }
}