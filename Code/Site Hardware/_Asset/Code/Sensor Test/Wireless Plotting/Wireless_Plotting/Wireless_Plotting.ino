#include <Wire.h>
#include <MechaQMC5883.h>
#include <SoftwareSerial.h>
SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin
#define setPin 7

MechaQMC5883 qmc;

long timerQ;
int Qtime = 1000;
float G;

String Baud = "AT+B2400";
String Ch = "AT+C001";

void setup() {
  Wire.begin();
  Serial.begin(2400);
  HC12.begin(2400);               // Serial port to HC12

  qmc.init();
  qmc.setMode(Mode_Continuous, ODR_200Hz, RNG_2G, OSR_512); // Imba Mode

  //  qmc.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_64); // Hemat Mode

  //  AT ();

  timerQ = millis();

}

void loop() {
  if (millis() - timerQ >= Qtime) {
    SensorRead();
        SerPrint ();
    HC12.print(G);
    timerQ = millis();

  }
}
