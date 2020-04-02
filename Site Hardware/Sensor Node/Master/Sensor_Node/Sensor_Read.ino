#include <Arduino.h>

//------------ Sensor Read ------------

float sensorRead() {
  magneto.init();
  magneto.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_256);
  delay(200);
  int x, y, z;
  float Gread;
  magneto.read(&x, &y, &z);

  return Gread = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

float sensorReadDetail() {
  float total, Gread;
  int avgNum = 10;

  // Counting for Averaging
  for (byte avgPos = 0; avgPos < avgNum; avgPos++) {
    magneto.init();
    magneto.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_512);
    delay(200);
    int x, y, z;
    float Gread;

    magneto.read(&x, &y, &z);
    Gread = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    total += Gread;
    delay(5);
  }
  return Gread = total / avgNum;
}
