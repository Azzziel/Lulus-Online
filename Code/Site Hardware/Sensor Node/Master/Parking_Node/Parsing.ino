
void Data_Parsing() {

  //-------- Parse the data
  char *myID = strtok(RF_message, "/");
  char *senderID = strtok(NULL, "/");
  char *Input = strtok(NULL, "/");
  char *Value = strtok(NULL, "#");
  //---------------------


  //-------- Check the data
  if (strcmp(ID, ID_sensor) == 0) {
    // The Possible Case
    //Start Condition
    if (strcmp(Input, "START") == 0) {
      AT_sleep ();
      flag_listen = 0;
      flag_start = 1;

      //Repeater ID
      strcpy (ID_rptr, IDreap);
      EEP_Write (20, ID_rptr);
    }

    //Acknowledge Condition
    if (strcmp(Input, "ACK") == 0) {
      AT_sleep ();
      flag_listen = 0;
    }
  }
  char RF_message[30];
  //---------------------
}
