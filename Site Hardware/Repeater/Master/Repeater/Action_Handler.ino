void actionHandler() {
  // ------- Repeater Begin
  if (actionCases == 1) {
    firstTime[0] = '0';
    // Write it to the eeprom
    EEPwrite (11, IDnextRepeater);
    EEPwrite (21, chSensorListen);
    EEPwrite (31, chSensorSend);
    EEPwrite (41, firstTime);
    EEPwrite (101, gatewayRoute);

    canListenSensor = true;
    canListenBouncer = true;
  }

  // ------- Receive Sensor Message
  else if (actionCases == 2) {
    // ACK Sensor Reply
    messageSendACK();

    // Send the sensor message to the next repeater
    //----Sending Routine
    messageNextRepeater(bufferIDsender, bufferProperty, bufferValue);
    isWaitingACK = true ;
    canListenBouncer = true;
    canListenSensor = false;    //Focus on waiting the ACK reply

    while (isWaitingACK) {
      waitingACK();
      messageNextRepeater(bufferIDsender, bufferProperty, bufferValue);
    }
    //------------
    canListenSensor = true;
    canListenBouncer = true;
  }

  // ------- Repeater as Message Bouncer
  else if (actionCases == 3) {
    // ACK Reply to the previous repeater
    messageSendACK();

    // Bounce the message to the next repeater
    //----Sending Routine
    messageAsBouncer();
    isWaitingACK = true ;
    canListenBouncer = true;
    canListenSensor = false;    //Focus on waiting the ACK reply

    while (isWaitingACK) {
      waitingACK();
      messageAsBouncer();
    }
    //------------
    canListenSensor = true;
    canListenBouncer = true;
  }
}

void messageSendACK() {
  // Make the data string
  char sendMessage[30] = "";
  strcpy (sendMessage, "<");
  strcat (sendMessage, bufferIDsender);  // Put the previous sender ID
  strcat (sendMessage, "/");
  strcat (sendMessage, IDrepeater);  // Append with this repeater ID
  strcat (sendMessage, "/ACK/1/>");

  delay(50);
  Serial.println(sendMessage);
  RFsensorSend.write(sendMessage);
  delay(50);
}

void messageAsBouncer() {
  // Make the data string
  char sendMessage[100] = "";
  strcpy (sendMessage, "<");
  strcat (sendMessage, bouncedData);
  strcat (sendMessage, ">");

  delay(50);
  Serial.println(sendMessage);
  RFsensorSend.write(sendMessage);
  delay(50);
}


void messageNextRepeater(char IDownerMessage[], char property[], char value[]) {
  // Make the data string
  char sendMessage[100] = "";
  strcpy (sendMessage, "<");
  strcat (sendMessage, IDnextRepeater);
  strcat (sendMessage, "/");
  strcat (sendMessage, IDownerMessage);  // Append with the message owner ID
  strcat (sendMessage, "/");
  strcat (sendMessage, property);  // Append the property
  strcat (sendMessage, "/");
  strcat (sendMessage, value);  // Append with the status
  strcat (sendMessage, "/>");

  delay(50);
  Serial.println(sendMessage);
  RFbouncerSend.write(sendMessage);
  delay(50);
}
