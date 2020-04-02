#include <Wire.h>
#include <MechaQMC5883.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "LowPower.h"
#include <SoftwareSerial.h>

#define setPin 7

MechaQMC5883 qmc;
Adafruit_BME280 bme;
SoftwareSerial HC12(7, 8); // HC-12 TX Pin, HC-12 RX Pin

// Deep Sleep Counting
long countNow;
long countRead = 150; // Every 20 minutes [20m x 60s / 8s sleep]

// Values
float G, tmp, hmd;
String Data;

String Baud = "AT+B2400";
String Ch = "AT+C001";

void setup() {
  Wire.begin();
  bme.begin(0x76);
  Serial.begin(2400);
  HC12.begin(2400);               // Serial port to HC12

  qmc.init();
  //  qmc.setMode(Mode_Continuous, ODR_200Hz, RNG_2G, OSR_512); // Imba Mode

  //  qmc.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_64); // Hemat Mode

  //  AT ();
  countNow = 150;
}

void loop() {

  if (countNow == countRead) {
    MagnetoRead();
    BMERead();

    //    SerPrint ();
    MakeString();
    Serial.println(Data);
    HC12.print(Data);

    countNow = 0;
  }
  else {
    countNow++;
    Serial.println(countNow);
  }
  delay (500);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
