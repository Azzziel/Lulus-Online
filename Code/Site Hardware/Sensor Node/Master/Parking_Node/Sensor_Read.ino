//------------ Sensor Read ------------

float Sensor_Read() {
  magneto.init();
  magneto.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_256);

  int x, y, z;
  float G_read;
  magneto.read(&x, &y, &z);

  return  G_read = sqrt (pow (x, 2) + pow (y, 2) + pow (z, 2));
}

float Sensor_Detailed() {

  float total = 0;
  int arrayPos;
  int arrayNum = 10;
  float G_read = 0;

  // Counting for Averaging
  for ( arrayPos = 0 ; arrayPos < arrayNum ; arrayPos++ ) {

    magneto.init();
    magneto.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_512);
    int x, y, z;
    float G_read;
    
    magneto.read(&x, &y, &z);
    G_read = sqrt (pow (x, 2) + pow (y, 2) + pow (z, 2));
    total += G_read;
    delay(5);
  }

  return  G = total / arrayNum;
}
