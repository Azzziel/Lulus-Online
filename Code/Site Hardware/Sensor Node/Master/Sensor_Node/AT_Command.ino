#include <Arduino.h>

void initATroutine() {
  digitalWrite(setAT_Pin, LOW);
  delay(50);
  RF.write(chSend);
  delay(50);
  RF.write(baud);
  delay(50);
  digitalWrite(setAT_Pin, HIGH);
  emptyingBuffer();

  // Sending Routine, Send the Reset Notification to Repeater
  dataSendRF("RST", "1");
  listenRoutine();
  // Wait until it get the ACK
  while (canListen) {
    dataSendRF("RST", "1");
    listenRoutine();
  }

  // Go to Listen mode
  channelAT(chListen);
  canListen = 1;
}

void channelAT(char channel[]) {
  digitalWrite(setAT_Pin, LOW);
  delay(50);
  RF.write(channel);
  delay(50);
  digitalWrite(setAT_Pin, HIGH);
  emptyingBuffer();
}

void sleepAT() {
  digitalWrite(setAT_Pin, LOW);
  delay(50);
  RF.write("AT+SLEEP");
  delay(50);
  digitalWrite(setAT_Pin, HIGH);
  emptyingBuffer();
}
