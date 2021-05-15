/*
  Environment Air monitor with MQTT sender

  Hardware: MHET Live, BME280, CCS811

  Requirement Library:
    Adafruit_Sensor Library
    Adafruit_BME280 Library
    SparkFun CCS811 Arduino Library
    PubSubClient
   
 */

#include <Adafruit_Sensor.h> // for BME280
#include <Adafruit_BME280.h> // for BME280
#include <SparkFunCCS811.h> 
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "AQM0802.h"       // LCD


const char SCLPIN = 18;   //I2C SCL
const char SDAPIN = 19;   //I2C SDA

const double SEALEVELPRESSURE_HPA = 1013.25;
const char AM_CSS_RST = 22;    //CCS811-RST
const char AM_CCS811_ADDR = 0x5A;


Adafruit_BME280 bme; 
CCS811 myCCS811(AM_CCS811_ADDR);
AQM0802 lcd;

float temperature;
float humid;
float pressure;
float altitude;
float CO2;
float TVOC;

int init_BME280() {
  unsigned status;  
  status = bme.begin(0x76, &Wire);
  return( status );
}

int init_CCS811() {
  pinMode(AM_CSS_RST, OUTPUT);
  digitalWrite(AM_CSS_RST, LOW);
  delay(1500);
  digitalWrite(AM_CSS_RST, HIGH);
  delay(1500);  

  return(myCCS811.begin(Wire));
}


void init_display() {
  lcd.begin(Wire);
  lcd.setcontrast(24); //contrast value range is 0-63, try 25@5V or 50@3.3V as a starting value
}



void setup(void)
{    
    Wire.begin(SDAPIN,SCLPIN);
    Serial.begin(115200);
            
    if (!init_BME280() ) {
      //tft.drawString("sensor NG", 0, 0, 2);
      Serial.print("BME280 NG");
    }
    if (!init_CCS811() ) {
      Serial.print("CCS811 NG");
    }
    init_display();
    
    lcd.setCursor(0, 0);  //LINE 1, ADDRESS 0

    delay(5000);    
}


void loop(void)
{
  temperature = bme.readTemperature();
  humid    = bme.readHumidity();
  pressure = bme.readPressure()/100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);  
  myCCS811.readAlgorithmResults();
  CO2  = myCCS811.getCO2();
  TVOC = myCCS811.getTVOC();

  delay(1000);
  Serial.print(temperature,1);
  Serial.print(",");   
  Serial.print(humid,0);
  Serial.print(",");   
  Serial.print(pressure,0);
  Serial.print(",");   
  Serial.print(CO2,0);
  Serial.print(",");     
  Serial.print(TVOC,0);
  Serial.println("");   

  lcd.setCursor(0, 0);
  lcd.print(temperature,1);
  lcd.print(" *C ");
  lcd.setCursor(1, 0);
  lcd.print(CO2,0);
  lcd.print(" ppm ");


}
