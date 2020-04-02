#include <SoftwareSerial.h>
#include <EEPROM.h>

// Array Lenght
const byte repID_len = 10;
const byte rouID_len = 100;
const byte main_len = 70;
const byte rcv_len = 30;
const byte send_len = 30;
const byte route_len = 50;

//ID Data
char repeaterID[repID_len];
char routeID[rouID_len];

// index
byte iRead;

// RF Data
SoftwareSerial RF(8, 9);
#define set_Pin 7
char Baud[] = "AT+B2400";
char chListen[] = "AT+C010";
char chSend[] = "AT+C017";
char mainData[main_len];
char receiveRF[rcv_len];
char sendRF[send_len];
char routeID[route_len] = "lalala";

void setup() {
  Serial.begin(2400);
  RF.begin(2400);

  EEP_Read (1, 10, repeaterID, repID_len);
}

void loop() {
  // Listening to any RF signal on the channel
  RF_Listen();
}
