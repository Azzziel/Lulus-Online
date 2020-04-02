#include <SoftwareSerial.h>
#include <EEPROM.h>

//ID Data
char IDrepeater[10];
char IDnextRepeater[10];
char gatewayRoute[70];


// HC12
SoftwareSerial RFsensorListen(3, 4);  // TX, RX module
#define setAT_SL_Pin 5
SoftwareSerial RFsensorSend(6, 7);
#define setAT_SS_Pin 8
SoftwareSerial RFbouncerListen(9, 10);
#define setAT_BL_Pin 11
SoftwareSerial RFbouncerSend(12, 13);
#define setAT_BS_Pin 14

char baud[] = "AT+B2400";

char chBouncerListen[] = "AT+C020";
char chBouncerSend[] = "AT+C027";
char chSensorListen[10];
char chSensorSend[10];

// Message Variables
char bouncedData[50];
char RFmessageReceive[50];
char bufferProperty[10];
char bufferValue[10];
char bufferIDsender[10];
byte iCount;
byte actionCases;
bool canListenSensor, canListenBouncer, canParse;
bool isWaitingACK;

// Others
char firstTime[1] = "0";


void setup() {
  Serial.begin(9600);
  Serial.println("It begin!");

  //========= Initialize HC12
  //============================
  RFsensorListen.begin(2400);
  RFsensorSend.begin(2400);
  RFbouncerListen.begin(2400);
  RFbouncerSend.begin(2400);

  pinMode(setAT_SL_Pin, OUTPUT);
  pinMode(setAT_SS_Pin, OUTPUT);
  pinMode(setAT_BL_Pin, OUTPUT);
  pinMode(setAT_BS_Pin, OUTPUT);

  //========= Read Existing EEPROM
  //============================
  EEPread(1, 10, IDrepeater);
  EEPread(11, 10, IDnextRepeater);
  EEPread(21, 10, chSensorListen);
  EEPread(31, 10, chSensorSend);
  EEPread(41, 1, firstTime);
  if (firstTime == '0') 
    EEPread(101, 70, gatewayRoute);   // It's to big and unpredictable to put a dummy first in the EEPROM

  //========= Starting Routine
  //============================
  // Check if this is the first time it deploy
  if (firstTime == '1')    //if its 'true'
    firstTimeHandler();
  else {
    initAT();
    resetRoutine();
  }
}

void loop() {
  listeningRoutine();
}


/* Things to be remembered:
    - After Action Handler, nyalain lagi "bisa denger" nya
*/
