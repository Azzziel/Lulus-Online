#include <Arduino.h>

#include <Wire.h>
#include <MechaQMC5883.h>
#include <SoftwareSerial.h>
#include "LowPower.h"
#include <EEPROM.h>

//ID Data
char IDsensor[10], IDrptr[10];

// Magneto Data
MechaQMC5883 magneto;
int G, Gidle, Gpark, GidleEEPnum;
char GidleEEP[10];
byte valueTrigger = 500;
byte valueIgnored = 100;
byte valueConsideredEEP = 200;
bool canPark, isBeginReading;

// RF Data
SoftwareSerial RF(7, 8);  // TX, RX module
#define setAT_Pin 6
char baud[] = "AT+B2400";
char chSend[] = "AT+C010";
char chListen[] = "AT+C017";
char RFmessage[30];
bool canListen;
byte iCount;

// Battery Data
#define batt_Pin A0
long battTrigger;
long battCount;


void setup() {
  Serial.begin(2400);
  RF.begin(2400);
  Wire.begin();
  pinMode(setAT_Pin, OUTPUT);
  Serial.println("It begin!");

  // Setting up the first flag
  canPark = 0;
  canListen = 1;

  // Read the existing EEPROM to get the sensor and repeater ID
  EEPread (1, 10, IDsensor);
  EEPread (11, 10, IDrptr);
  EEPread (21, 10, GidleEEP);

  Serial.print ("ID Sensor: ");
  Serial.print (IDsensor);
  Serial.print ("   | ID Rptr: ");
  Serial.print (IDrptr);
  Serial.print ("   | G idle: ");
  Serial.println (GidleEEP);

  // Set the counter value to read Batt
  // 24hours(86400s)/ 4s(sleep) = 21600. So it's +- every 1 day
  battTrigger = random(21000, 22000);

  // Sending a RESET notification to gateway and start listen for a BEGIN command
  initATroutine();
}

void loop() {

  //=========================
  //-------- Start State
  //=========================
  if (canListen) {
    // Keep listening
    listenRF();
  }

  //=========================
  //-------- Normal State
  //=========================

  if (!canListen) {

    // First time write G-Idle
    if (isBeginReading)
    {
      writeGidle();
      isBeginReading = 0;

      Serial.print("nilai G idle: ");
      Serial.println(Gidle);
    }

    // Checking the magneto
    G = sensorRead();
    Serial.print("nilai G : ");
    Serial.println(G);

    checkParkCondition();

    //Battery Routine
    battRoutine();

    // All is done, now sleep!
    delay(200);
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  }
}

// -----THINGS TO ADD
// Version: improve dengan bikin class & ubah nama
