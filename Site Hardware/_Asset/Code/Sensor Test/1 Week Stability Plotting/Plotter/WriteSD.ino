void WriteSD() {
  myFile = SD.open ("Sensor.txt", FILE_WRITE);

  if (myFile) {
    myFile.print (timeNow);
    myFile.print ("/");
    myFile.println (Message);
    myFile.close ();
  }

  Serial.print (timeNow);
  Serial.print ("/");
  Serial.println (Message);

}
