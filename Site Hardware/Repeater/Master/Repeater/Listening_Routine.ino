void listeningRoutine() {
  // Listening for an incoming message
  if (canListenSensor)
    listenSensorRF();
  if (canListenBouncer)
    listenBouncerRF();

  // Handling the messasge
  if (canParse) {
    dataParsing();
    actionHandler();
  }
}


void listenSensorRF() {
  bool canStartRead, canStopRead;
  while (RFsensorListen.available()) {
    char buffWord = RFsensorListen.read();
    delay(2);

    incomingRFhandler(buffWord, canStartRead, canStopRead);
  }
}

void listenBouncerRF() {
  bool canStartRead, canStopRead;
  while (RFbouncerListen.available()) {
    char buffWord = RFbouncerListen.read();
    delay(2);

    incomingRFhandler(buffWord, canStartRead, canStopRead);
  }
}

void incomingRFhandler(char buffWord[], bool canStartRead, bool canStopRead) {
  //Check if the incoming message is '>'
  if (buffWord == (char)0x3e) {
    canStartRead = false;
    canStopRead = true;
  }

  // Start reading if the flag already true
  if (canStartRead && iCount < 50) {
    RFmessageReceive[iCount] = buffWord;
    iCount++;
  }
  // It's time to process the data
  else if (!canStartRead && canStopRead) {
    Serial.println(RFmessageReceive);
    canParse = true;
    canStopRead = false;
    canListenSensor = false;
    canListenBouncer = false;
  }

  //Check if the incoming message is '<'
  if (buffWord == (char)0x3c) {
    canStartRead = true;
    char RFmessageReceive[50] = "";
    iCount = 0;
  }
}


void waitingACK() {
  long currentTime = millis();
  int listenTime = random(5000, 12000);
  Serial.print("nguping selama (random): ");
  Serial.println(listenTime);

  while (millis() - currentTime <= listenTime) {
    listeningRoutine();
  }
}


void emptyingBuffer() {
  while (RFsensorListen.available()) {
    Serial.write(RFsensorListen.read());
    delay(2);
  }

  while (RFsensorSend.available()) {
    Serial.write(RFsensorSend.read());
    delay(2);
  }

  while (RFbouncerListen.available()) {
    Serial.write(RFbouncerListen.read());
    delay(2);
  }

  while (RFbouncerSend.available()) {
    Serial.write(RFbouncerSend.read());
    delay(2);
  }
}
