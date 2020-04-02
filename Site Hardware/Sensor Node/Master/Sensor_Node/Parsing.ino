#include <Arduino.h>

void dataParsing() {
  //-------- Parse the data
  char *myID = strtok(RFmessage, "/");
  char *pairID = strtok(NULL, "/");
  char *property = strtok(NULL, "/");
  char *value = strtok(NULL, "/");
  //---------------------

  Serial.println("Here are the result");
  Serial.print("myID : ");
  Serial.println(myID);
  Serial.print("pairID : ");
  Serial.println(pairID);
  Serial.print("Property : ");
  Serial.println(property);
  Serial.print("Value : ");
  Serial.println(value);

  //-------- Check the data
  if (!strcmp(myID, IDsensor)) {
    //------ The Possible Case

    // Start Condition
    if (!strcmp(property, "BEGIN")) {
      Serial.println("Olrait gw mulai");
      canListen = 0;
      isBeginReading = 1;
      sleepAT();

      // Repeater ID
      if (!strcmp(pairID, "CENTRAL")) {
        //-------- Parse the subvalue
        char *Vprop = strtok(value, ":");
        char *Vval1 = strtok(NULL, "#");

        // Check the repeater ID
        if (strcmp(Vval1, IDrptr) != 0) {
          strcpy(IDrptr, value);
          EEPwrite(11, IDrptr);
          Serial.print("Repeater ID updated: ");
          Serial.println(IDrptr);
        }
      }
    }

    // Acknowledge Condition
    else if (!strcmp(property, "ACK")) {
      Serial.println("ACK yoo sipp");
      sleepAT();
      canListen = 0;
    }
  }

  // emptying the RFmessage
  memset(RFmessage, '\0', sizeof(RFmessage));
  //---------------------
}
