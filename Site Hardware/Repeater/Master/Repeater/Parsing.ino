
void dataParsing() {

  //========= Parse the data
  //============================

  // Make a copy because once it parsed with strtok, the whole string "chopped" with null char
  char buffMessage[50];
  strcpy(buffMessage, RFmessageReceive);

  char *myID = strtok(buffMessage, "/");
  char *senderID = strtok(NULL, "/");
  char *property = strtok(NULL, "/");
  char *value = strtok(NULL, "/");
  //---------------------

  Serial.println("Here are the result");
  Serial.print("myID : ");
  Serial.println(myID);
  Serial.print("senderID : ");
  Serial.println(senderID);
  Serial.print("Property : ");
  Serial.println(property);
  Serial.print("Value : ");
  Serial.println(value);


  //========= Check the data
  //============================

  // If the received myID is the repeater ID
  if (!strcmp(myID, IDrepeater)) {

    //------ The Possible Case
    //=======================================

    //------Sensor Message Condition
    if (strcmp(property, "BEGIN") != 0) {            // BEGIN is the only possible message beside all the message from sensor. So anything else valid
      actionCases = 2;
      strcpy (bufferIDsender, senderID);
      strcpy (bufferProperty, property);
      strcpy (bufferValue, value);
    }

    //------Get an ACK from the next Repeater
    else if (!strcmp(property, "ACK")) {
      isWaitingACK = false;
    }

    //------Repeater First Time Condition
    else if (!strcmp(property, "BEGIN")) {       // During the first time deploy only
      actionCases = 1;

      //------Parse the subvalue
      /* Ex. AT+C010#AT+C017#FF2A:57BC:8D5B
        Ch Sensor Listen # Ch Sensor Send # Route to Gateway
      */
      char *chSL = strtok(value, "#");
      char *chSS = strtok(NULL, "#");
      char *gtwRoute = strtok(NULL, "#");

      //Get the first Route as it's next repeater ID
      char buffMessage[50];
      strcpy(buffMessage, gtwRoute);
      char *nextID = strtok(buffMessage, ":");

      // Copy the string to the real char array
      strcpy(chSensorListen, chSL);
      strcpy(chSensorSend, chSS);
      strcpy(gatewayRoute, gtwRoute);
      strcpy(IDnextRepeater, nextID);
      //------------------
    }
  }

  // Else, the myID must be a Route
  //------Another Repeater Message Condition
  else {
    char *ID1 = strtok(myID, ":");

    // If the first ID in the route is this repeater ID, it means this message is yours
    if (!strcmp(ID1, IDrepeater)) {
      actionCases = 3;

      // For ACK to the previous repeater
      strcpy (bufferIDsender, senderID);

      // For bouncing to the next repeater
      // Remove this repeater ID from the message and send the rest
      char *ID = strtok(buffMessage, ":");
      char *content = strtok(NULL, '\0');
      strcpy (bouncedData, content);
    }
  }
  canParse = false;
}

//================================
// Action Handler Cases
// 1 :  Repeater Begin
// 2 :  Receive Sensor Message
// 3 :  Repeater as Message Bouncer
//================================
