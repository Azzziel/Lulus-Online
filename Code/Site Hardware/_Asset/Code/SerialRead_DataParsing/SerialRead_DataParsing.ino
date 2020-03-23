char RF_message[30];
int iCount = 0;
//bool flagRead, flagStop;

void setup() {
  Serial.begin(9600);

}

void loop() {
  bool flagRead, flagStop;
  while (Serial.available()) {
    char buffWord = Serial.read();

    //Check if the incoming message is '>'
    if (buffWord == (char)0x3e) {
      flagRead = false;
      flagStop = true;
    }

    // Start reading if the flag already true
    if (flagRead) {
      RF_message[iCount] = buffWord;
      iCount++;
    }
    // It's time to process the data
    else if (!flagRead && flagStop) {
      Serial.println(RF_message);
      flagStop = false;
    }

    //Check if the incoming message is '<'
    if (buffWord == (char)0x3c) {
      flagRead = true;
      memset (RF_message, '\0', sizeof(RF_message));
      iCount = 0;
    }
  }
}
