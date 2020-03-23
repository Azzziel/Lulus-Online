#include <SoftwareSerial.h>

SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin

#define setPin 7

String Baud = "AT+B9600";
String Ch = "AT+C001";
String Message;

void setup() {
  Serial.begin(2400);             // Serial port to computer
  HC12.begin(2400);               // Serial port to HC12

  pinMode(setPin, OUTPUT);
  digitalWrite(setPin, HIGH);

  //  AT ();
}

void loop() {
  while (HC12.available()) {
    Message = HC12.readString();
    Serial.println(Message);
  }
}
