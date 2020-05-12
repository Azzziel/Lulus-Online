#include <SoftwareSerial.h>
#include <Wire.h>

#include <Node_HC12.h>
#include <MechaQMC5883.h>

const unsigned int RX = 8;
const unsigned int TX = 9;
const unsigned int SET = 7;

SoftwareSerial serial(RX, TX);
Node_HC12 hc(&serial, SET);

MechaQMC5883 qmc;

void setup() {
  //  Serial.begin(115200);
  if (!hc.begin(2400, 1))
    halt();
  Serial.begin(115200);

  Wire.begin();
  qmc.init();
  qmc.setMode(Mode_Continuous, ODR_200Hz, RNG_2G, OSR_512);
}

void loop() {
  const unsigned long interval = 500UL;
  static unsigned long previousMillis = millis() - interval;
  if (millis() - previousMillis >= interval) {
    int x, y, z;
    qmc.read(&x, &y, &z);

    const double magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    String toSend;
    toSend += magnitude;
    toSend += ' ';
    toSend += x;
    toSend += ' ';
    toSend += y;
    toSend += ' ';
    toSend += z;

    Serial.println(toSend);
    serial.print(toSend);

    previousMillis = millis();
  }
}

void halt() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  while (true)
    delay(0);
}
