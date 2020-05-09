#include <SoftwareSerial.h>
#include <Node_HC12.h>

const unsigned int RX = 8;
const unsigned int TX = 9;
const unsigned int SET = 7;

SoftwareSerial serial(RX, TX);
Node_HC12 hc(&serial, SET);

void setup() {
  if (!hc.begin(2400, 1))
    halt();

  Serial.begin(115200);
}

void loop() {
  if (serial.available()) {
    String buffer;

    unsigned long previousMicros = micros();
    while (true) {
      if (micros() - previousMicros < 3000UL) {
        if (serial.available()) {
          buffer += static_cast<char>(serial.read());
          previousMicros = micros();
        }
      }
      else {
        break;
      }
    }

    Serial.println(buffer);
  }
}

void halt() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  while (true)
    delay(0);
}
