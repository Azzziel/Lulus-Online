#include <Arduino.h>

void dataSendRF(char property[], char value[]) {
  channelAT (chSend);

  // Make the data string
  char sendRF[30];
  strcpy (sendRF, "<");  // Put the sensor ID
  strcat (sendRF, IDrptr);  // Put the sensor ID
  strcat (sendRF, "/");
  strcat (sendRF, IDsensor);  // Append this sensor ID
  strcat (sendRF, "/");
  strcat (sendRF, property);  // Append the property
  strcat (sendRF, "/");
  strcat (sendRF, value);  // Append with the status
  strcat (sendRF, "/>");

  delay(50);
  Serial.println(sendRF);
  RF.write(sendRF);
  delay(50);
  memset (sendRF, '\0', sizeof(sendRF));
}

void listenRF() {
  bool canStartRead, canStopRead;
  while (RF.available()) {
    char buffWord = RF.read();
    delay(2);
    
    //Check if the incoming message is '>'
    if (buffWord == (char)0x3e) {
      canStartRead = false;
      canStopRead = true;
    }

    // Start reading if the flag already true
    if (canStartRead) {
      RFmessage[iCount] = buffWord;
      iCount++;
    }
    // It's time to process the data
    else if (!canStartRead && canStopRead) {
      Serial.println(RFmessage);
      dataParsing();
      canStopRead = false;
    }

    //Check if the incoming message is '<'
    if (buffWord == (char)0x3c) {
      canStartRead = true;
      memset (RFmessage, '\0', sizeof(RFmessage));
      iCount = 0;
    }
  }
}

void listenRoutine() {
  channelAT(chListen);
  canListen = 1;
  long currentTime = millis();
  int listenTime = random(5000, 12000);
  Serial.print("nilai random: ");
  Serial.println(listenTime);
  while ((millis() - currentTime <= listenTime) && canListen == 1) {
    listenRF();
  }
}

void emptyingBuffer() {
  while (RF.available()) {
    Serial.write(RF.read());
    delay(2);
  }
}
