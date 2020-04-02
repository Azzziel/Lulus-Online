void initAT () {
  // Entering AT Mode
  digitalWrite(setAT_SL_Pin, LOW);
  digitalWrite(setAT_SS_Pin, LOW);
  digitalWrite(setAT_BL_Pin, LOW);
  digitalWrite(setAT_BS_Pin, LOW);
  delay (100);

  // Setting Up the Channel
  RFsensorListen.write(chSensorListen);
  delay (50);
  RFsensorSend.write(chSensorSend);
  delay (50);
  RFbouncerListen.write(chBouncerListen);
  delay (50);
  RFbouncerSend.write(chBouncerSend);
  delay (50);

  // Setting Up the Baud Rate
  RFsensorListen.write(baud);
  delay (50);
  RFsensorSend.write(baud);
  delay (50);
  RFbouncerListen.write(baud);
  delay (50);
  RFbouncerSend.write(baud);
  delay (50);

  // Exit the AT Mode
  digitalWrite(setAT_SL_Pin, HIGH);
  digitalWrite(setAT_SS_Pin, HIGH);
  digitalWrite(setAT_BL_Pin, HIGH);
  digitalWrite(setAT_BS_Pin, HIGH);
  delay (100);

  emptyingBuffer();   // Emptying the RF buffer
}


void resetRoutine() {
  //   Sending Routine, Send the Reset Notification to Repeater
  messageNextRepeater(IDrepeater, "RST", "1");
  isWaitingACK = true ;
  canListenBouncer = true;
  canListenSensor = false;    //Focus on waiting the ACK reply

  while (isWaitingACK) {
    waitingACK();
    messageNextRepeater(IDrepeater, "RST", "1");
  }
  canListenSensor = true;
  canListenBouncer = true;
}


void firstTimeHandler() {
  initAT();

  while (!canParse) {
    listenBouncerRF();
  }
  dataParsing();
  actionHandler();
}
