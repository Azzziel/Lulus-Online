#include <Wire.h>
#include <MechaQMC5883.h>
#include <SoftwareSerial.h>
#include "LowPower.h"
#include <EEPROM.h>

//ID Data
char ID_sensor[10], ID_rptr[10];

// Magneto Data
MechaQMC5883 magneto;
float G, G_idle, G_park;
int trigger_park = 500;
bool flag_park, flag_check, flag_recheck, flag_start;

// Timer
int timer_read = 1000;
long read_prev;
int timer_recheck = 5000;
long recheck_prev;

// RF Data
SoftwareSerial RF(8, 9);
#define set_Pin 7
char Baud[] = "AT+B2400";
char ch_send[] = "AT+C010";
char ch_listen[] = "AT+C017";
String Data;
char RF_message[30];
int listen_time;
bool flag_listen;

// Battery Data
int batt_max = 2000;  // 24hours/4s(sleep) = 2160. So it's +- every 1 day
int batt_count;
#define batt_Pin A0
float batt_volt;

// Flag Class
class Flag {
public:
bool beginRoutine, park, check, makingSure, listenRF;
};

void setup() {
  Wire.begin();
  Serial.begin(2400);
  RF.begin(2400);

  flag_park = 0;
  flag_listen = 1;

  // EEPROMing
  EEP_Read (1, 10, ID_sensor);
  EEP_Read (21, 30, ID_rptr);

  AT_first();  // For Sending a RESET notification to repeater and start listen

}

void loop() {

  //-------- Start State
  if (flag_listen == 1) {
    RF_Listen();
  }
  //---------------------

  //-------- Normal State

  if (flag_listen == 0) {

    // First time write G-Idle
    if (flag_start == 1)
    {
      Write_Gidle();
      flag_start = 0;
      flag_check = 1;
    }

    // Checking the magneto
    if (flag_check == 1)
      G = Sensor_Read();

    Check_Condition();

    // Battery Routine
    Batt_Routine();

    // All is done, now sleep!
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  }
  //---------------------
}
