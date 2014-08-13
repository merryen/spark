// This #include statement was automatically added by the Spark IDE.
#include "sd-card-library/sd-card-library.h"

// This #include statement was automatically added by the Spark IDE.
#include "SparkIntervalTimer/SparkIntervalTimer.h"

// Libraries
#include "application.h"
#include "SHT1x.h"
//#include "spark_disable_wlan.h"
//#include "spark_disable_cloud.h"

// Defines
/*
WiFi_Status_TypeDef last_wifi_status;
TCPServer server = TCPServer(23);
TCPClient client;*/
IntervalTimer myTimer;

// Constants
const uint8_t chipSelect = A2;    // Also used for HARDWARE SPI setup
const uint8_t mosiPin = A5;
const uint8_t misoPin = A4;
const uint8_t clockPin = A3;
const int testPin = D5;
int ledState = LOW;
volatile unsigned long blinkCount = 0; // use volatile for shared variables

// variables
int ecgPin = A0;
int z = 0;
int l = 1;
//bool z = 0;
//bool l = 1;
int ecgReading[2][512] = {{0},{0}};
int counter = 0;
File ecgFile;


void setup() {
/*  last_wifi_status = WiFi.status();
    WiFi.on();
    server.begin(); */
    Serial.begin(9600);
    SD.begin(mosiPin, misoPin, clockPin, chipSelect);
    pinMode(testPin, OUTPUT);
    myTimer.begin(ecgData, 5000, uSec);  // collect ecgData every 2mili seconds
}
/*
void swap(){
    if (z == 0){
        z = 1;
        l = 0;
    } else{
        z = 0;
        l = 1;
    }
}

void swap_boole(){
    z = !z;
    l = !z;
}*/

void ecgData(){
    ecgReading[z][counter] = analogRead(ecgPin);
    Serial.println(ecgReading[z][counter]);
    counter = counter +1;
    if (ledState == LOW) {
        ledState = HIGH;
    } else {
        ledState = LOW;
    }
    digitalWrite(testPin, ledState);
}

void saveECG(){
    l = 1 - z;
    ecgFile = SD.open("ECG.txt", FILE_WRITE);
    Serial.println("Swap ");
    Serial.print(l);
    Serial.println(z);
    if (ecgFile) {
        Serial.println("Starts saving");
        for(int i = 0; i<512; i++){
            ecgFile.print(ecgReading[l][i]);
            ecgFile.print(";");
        }
        ecgFile.close();
        Serial.println("Finished saving");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
}
void loop() {
    if (counter >= 512){
        noInterrupts();
        z = 1 - z;
        counter = 0;
        interrupts();
        saveECG();
    }


}
