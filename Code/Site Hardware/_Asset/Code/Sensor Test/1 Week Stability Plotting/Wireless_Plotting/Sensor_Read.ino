void MagnetoRead() {
  int x, y, z;
  qmc.init();
  qmc.setMode(Mode_Standby, ODR_10Hz, RNG_2G, OSR_256);
  qmc.read(&x, &y, &z);
  G = sqrt (pow (x, 2) + pow (y, 2) + pow (z, 2));
}

void BMERead() {
  tmp = bme.readTemperature();
  hmd = bme.readHumidity();
}
