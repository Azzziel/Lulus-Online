void RF_Listen() {
  while (RF.available()) {
    char buffWord = Serial.read();

    if (buffWord != (char)0x0A) {
      receiveRF[iRead] = buffWord;
      iRead++;
    }
    else {
      dataParsing();
      iRead = 0;
    }
  }
}

void Sensor_Ack(char receiverID) {
  AT (chSend);    // Set to listen mode

  // Make the data string
  strcpy (sendRF, receiverID);  // Put the sensor ID
  strcat (sendRF, "/");
  strcat (sendRF, repeaterID);  // Append this repeater ID
  strcat (sendRF, "/");
  strcat (sendRF, "ACK/1#");  // Append the ack information

  // Sending it through HC12
  RF.write(sendRF);
  memset (sendRF, '\0', send_len);   // Emptying the message string

  AT (chListen); // Back to listen mode

}
