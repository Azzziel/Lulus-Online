#include <SoftwareSerial.h>
SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin

int counter = 0;
long cTimer;

void setup() {
//  Serial.begin(9600);             // Serial port to computer
  HC12.begin(2400);               // Serial port to HC12

  cTimer = millis();
}

void loop() {

  if (millis() - cTimer > 1000) {
    HC12.print(counter);
    HC12.print('_');
    HC12.print("Lorem ipsum dolo");
    HC12.print('\n');

    ++counter;
    cTimer = millis();
//    Serial.println(counter);
  }
}
