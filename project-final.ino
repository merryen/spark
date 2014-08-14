// Libraries
#include "application.h"
#include "SHT1x.h"
#include "SparkIntervalTimer/SparkIntervalTimer.h"
#include "sd-card-library/sd-card-library.h"
//#include "spark_disable_wlan.h"
//#include "spark_disable_cloud.h"

// Wifi Defines
//WiFi_Status_TypeDef last_wifi_status;
//TCPServer server = TCPServer(23);
//TCPClient client;

// Constants
const uint8_t chipSelect = A2;              // Also used for HARDWARE SPI setup
const uint8_t mosiPin = A5;
const uint8_t misoPin = A4;
const uint8_t clockPin_2 = A3;

const int Zout = A6;
const int Xout = A7;
const int Yout = A1;
const int testPin = D5;

// Test
int ledState = LOW;
volatile unsigned long blinkCount = 0;      // use volatile for shared variables

//Delay timer to create a non-delay() x-second sampling timer
unsigned long wait = millis();
const unsigned long waittime = 5000L;


// Specify data and clock connections and instantiate SHT1x object
#define dataPin  D0
#define clockPin D1
SHT1x sht1x(dataPin, clockPin);

// ECG variables
int ecgPin = A0;
int z = 0;
int l = 1;
int ecgReading[2][500] = {{0},{0}};
int counter = 0;
int counter_temp = 0;
int counter_acc = 0;
IntervalTimer myTimer;

// Accelerometer variables
int Xout_read;
int Yout_read;
int Zout_read;

// SD card
File ecgFile;
File tempFile;
File accFile;

void setup() {
    //last_wifi_status = WiFi.ready();
   // WiFi.on();
   // Serial.println(WiFi.localIP());
    //server.begin();
    Serial.begin(9600);
    SD.begin(mosiPin, misoPin, clockPin_2, chipSelect);
    pinMode(testPin, OUTPUT);
    myTimer.begin(getEcg, 4000, uSec);  // collect ecgData every 5mili seconds
}

void getEcg(){
    ecgReading[z][counter] = analogRead(ecgPin);
    //Serial.println(ecgReading[z][counter]);
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
    if (ecgFile) {
        Serial.println("Starts saving ECG");
        for(int i = 0; i<512; i++){
            ecgFile.print(ecgReading[l][i]);
            ecgFile.print(";");
        }
        ecgFile.close();
        Serial.println("Finished saving ECG");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening ECG file");
    }
}

void getAcc(){
    Xout_read = analogRead(Xout);
    Yout_read = analogRead(Yout);
    Zout_read = analogRead(Zout);
    
    Serial.print("X = ");
    Serial.print(Xout_read);
    Serial.print(" ");
    Serial.print("Y = ");
    Serial.print(Yout_read);
    Serial.print(" ");
    Serial.print("Z = ");
    Serial.println(Zout_read);
    
    // Print the values to the SD card
    accFile = SD.open("Acc.txt", FILE_WRITE);
    if (accFile) {
        Serial.println("Starts saving acc");
        accFile.print("X = ");
        accFile.print(Xout_read);
        accFile.print(" ");
        accFile.print("Y = ");
        accFile.print(Yout_read);
        accFile.print(" ");
        accFile.print("Z = ");
        accFile.println(Zout_read);
        accFile.close();
        Serial.println("Finished saving acc");
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening Acc file");
    }
}

void getTemp(){
    float temp_c;
    //float temp_f;
    float humidity;

    if (millis() > wait) {
        wait = millis() + waittime;

        // Read values from the sensor
        temp_c = sht1x.readTemperatureC();
        humidity = sht1x.readHumidity();
 
        // Print the values to the serial port
        Serial.print("Temperature: ");
        Serial.print(temp_c, DEC);
        Serial.print("C");
        Serial.print(" ");
        //Serial.print(temp_f, DEC);
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println("%");
        
        // Print the values to the SD card
        tempFile = SD.open("Temp.txt", FILE_WRITE);
        if (tempFile) {
            Serial.println("Starts saving temp");
            tempFile.print("Temperature: ");
            tempFile.print(temp_c, DEC);
            tempFile.print("C");
            tempFile.print(" ");
            tempFile.print("Humidity: ");
            tempFile.print(humidity);
            tempFile.println("%");
            tempFile.close();
            Serial.println("Finished saving temp");
        } else {
            // if the file didn't open, print an error:
            Serial.println("error opening Temp file");
        }
        
    }
}

void loop() {
    if (counter >= 500){
        noInterrupts();
        z = 1 - z;
        counter = 0;
        interrupts();
        counter_acc = counter_acc +1;
        saveECG();
    }
    
    if (counter_acc >= 12){
        counter_temp = counter_temp +1;
        counter_acc = 0;
        getAcc();
    }
    
    if (counter_temp >=2){
        getTemp();
        counter_temp = 0;
    }
    
}