
void dataParsing() {

  //-------- Parse the data
  char *myID = strtok(receiveRF, "/");
  char *senderID = strtok(NULL, "/");
  char *Info = strtok(NULL, "/");
  char *Value = strtok(NULL, "#");
  //---------------------
  callbackDesicion(myID, senderID, Info, Value);

    memset (receiveRF, '\0', rcv_len);   // Emptying the message string
  //---------------------
}

void callbackDesicion(char myID[], char senderID[], char Info[], char Value[]) {

  //-------- Check the data
  if (strcmp(myID, repeaterID) == 0) {

    // The Possible Case
    //1. Start Condition
    if (strcmp(Info, "BEGIN") == 0) {
      // Parse the Gateway Route
      routeID_Parsing(Value);
      // Save the data to the EEPROM
    }

    //2. Sensor Status Condition
    if (strcmp(Info, "CAR") == 0 || (strcmp(Info, "BATT") == 0)) {

      // Combine the routing path + relayed data
      strcpy (mainData, routeID);  // Put the routing data as the first variable
      strcat (mainData, "/");
      strcat (mainData, senderID);  // Append the sensor ID
      strcat (mainData, ":");
      strcat (mainData, Info);  // Append the Information title
      strcat (mainData, ":");
      strcat (mainData, Value);  // Append the Information value

      // Send to the Bouncer uC

      // Acknowledge back to the sensor
      Sensor_Ack(senderID);
    }
  }
}

void routeID_Parsing(char Value[]) {
  //-------- Parse the data
  char *Title = strtok(Value, ":");   // Will contain a "ROUTE" string

  // Check how many ':' in the route data
  char *route1 = strtok(NULL, ":");
  if (route1 != 0) {
    // Continue to next route
    char *route2 = strtok(NULL, ":");
    if (route2 != 0) {
      // Continue to next route
    }
    else {
      char *route2 = strtok(NULL, "#");
    }
  }
  else {
    char *route1 = strtok(NULL, "#");
  }
}



// Make an array of routing IDs and put the data inside it's object. Number of object depends on the previous index

//----------
//  char *r1ID = strtok(NULL, "/");
}
