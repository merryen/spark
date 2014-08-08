// This #include statement was automatically added by the Spark IDE.
#include "SHT1x.h"
#include "application.h"

/**
 * ReadSHT1xValues
 *
 * Read temperature and humidity values from an SHT1x-series (SHT10,
 * SHT11, SHT15) sensor.
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au>
 * www.practicalarduino.com
 */
 
unsigned long wait = millis();              //Delay timer to create a non-delay() x-second sampling timer
const unsigned long waittime = 5000L;


// Specify data and clock connections and instantiate SHT1x object
#define dataPin  D0
#define clockPin D1
SHT1x sht1x(dataPin, clockPin);

void setup()
{
   Serial.begin(9600); // Open serial connection to report values to host
   Serial.println("Starting up");
}
 
void loop()
{
  float temp_c;
  float temp_f;
  float humidity;

  if (millis() > wait) {
    wait = millis() + waittime;

    // Read values from the sensor
    temp_c = sht1x.readTemperatureC();
    temp_f = sht1x.readTemperatureF();
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
    }
 
}