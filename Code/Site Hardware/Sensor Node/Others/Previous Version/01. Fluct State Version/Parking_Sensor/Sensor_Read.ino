//------------ Sensor Read ------------

void Sensor_Read () {

  float total = 0;
  int arrayPos;
  int arrayNum = 10;
  float G_read = 0;
  float G_before = 0;
  G_prev = G;

  // Counting for Averaging
  for ( arrayPos = 0 ; arrayPos < arrayNum ; arrayPos++ ) {
    magneto.read(&x, &y, &z);
    G_read = sqrt (pow (x, 2) + pow (y, 2) + pow (z, 2));
    total += G_read;
    delay(50);

    // Betulin nilai awal
    if (arrayPos == 0) {
      G_before = G_read;
    }

    // Checking potentially fluctuation
    if (abs (G_read - G_before) > trigger_fluct && flag_fluct == 0) {
      
      counting_fluct += 1;
      countfl_prev = millis ();

//      Serial.println ("Kayaknya bakal naik");
 //     Serial.println (counting_fluct);
    }
//    G_before = G_read;
  }


  G = total / arrayNum;

  // Print Reading Data in Serial
  Serial_Read();

  // Turning down false alarm (counting fluct)
  False_Fluct();

  // Setting a G value based on current state
  Gset_Value();


}
