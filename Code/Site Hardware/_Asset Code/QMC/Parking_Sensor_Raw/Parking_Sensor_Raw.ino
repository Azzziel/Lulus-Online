#include <Wire.h>
#include <MechaQMC5883.h>

MechaQMC5883 qmc;

void setup() {
  Wire.begin();
  Serial.begin(9600);
//  qmc.init();
  //  qmc.setMode(Mode_Continuous, ODR_200Hz, RNG_2G, OSR_512); // Imba Mode

//  qmc.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_64); // Hemat Mode



}

void loop() {
  int x, y, z;
  qmc.init();
  qmc.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_256);
  qmc.read(&x, &y, &z);

  float G;

  G = sqrt (pow (x, 2) + pow (y, 2) + pow (z, 2));

  //  Serial.print("x: ");
  //  Serial.print(x);
  //  Serial.print(" y: ");
  //  Serial.print(y);
  //  Serial.print(" z: ");
  //  Serial.print(z);
  //  Serial.println();
  Serial.print(" G: ");
  Serial.print(G);
  Serial.println();

  delay(1000);
}
