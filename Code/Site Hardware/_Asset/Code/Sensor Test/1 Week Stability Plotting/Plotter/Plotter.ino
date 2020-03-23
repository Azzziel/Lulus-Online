#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include "RTClib.h"

SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin
File myFile;
RTC_DS1307 rtc;

#define csPin 10 // Pin 10 on Arduino Uno
#define setPin 7

String Baud = "AT+B9600";
String Ch = "AT+C001";
String Message;
String timeNow;

void setup() {
  Serial.begin(2400);             // Serial port to computer
  HC12.begin(2400);               // Serial port to HC12

  pinMode(csPin, OUTPUT);
  SD.begin(csPin);

  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Create File
  myFile = SD.open ("Sensor.txt", FILE_WRITE);

  //Make Title
  if (myFile) {
    // Write to file
    myFile.println("Time / G / Temp / Hum");
    myFile.close(); // close the file
  }

  // Reading the file
  myFile = SD.open("Sensor.txt");
  if (myFile) {
    Serial.println("Read:");
    // Reading the whole file
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  }
  else {
    Serial.println("error opening test.txt");
  }

}

void loop() {
  while (HC12.available()) {
    Message = HC12.readString();
    Serial.println(Message);
    RTCcheck();
    WriteSD();
  }

//  while (Serial.available()) {
//    Message = Serial.readString();
//    Serial.println(Message);
//    RTCcheck();
//    WriteSD();
//  }

}
