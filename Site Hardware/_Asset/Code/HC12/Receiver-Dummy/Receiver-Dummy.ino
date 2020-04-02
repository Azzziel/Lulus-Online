#include <SoftwareSerial.h>
SoftwareSerial HC12(7, 8); // HC-12 TX Pin, HC-12 RX Pin

// HC12 Setting
#define set_Pin 6
char ch_send[] = "AT+C010";   // Sensor send, receiver listen
char ch_listen[] = "AT+C017";
char Baud[] = "AT+B2400";

//ID Setting
char ID[15];
long ID_hx = 0x4f23;    // Ubah ini


void setup() {
  Serial.begin(2400);             // Serial port to computer
  HC12.begin(2400);               // Serial port to HC12
  pinMode(set_Pin, OUTPUT);

  Serial.println("start!");
  AT_init();
  sprintf(ID, "%ld", ID_hx);
  Serial.println(ID);
}

void loop() {
  while (HC12.available()) {        // If HC-12 has data
    Serial.write(HC12.read());      // Send the data to Serial monitor
  }
  while (Serial.available()) {      // If Serial monitor has data
    HC12.write(Serial.read());      // Send that data to HC-12
  }
}

void AT_init() {
  digitalWrite(set_Pin, LOW);
  delay (50);                         // Delay min. 50ms (below will be "ERROR")
  HC12.write(ch_listen);
  //  HC12.write(ch_send);
  delay (50);
  HC12.write(Baud);
  delay (50);
  digitalWrite(set_Pin, HIGH);
}
