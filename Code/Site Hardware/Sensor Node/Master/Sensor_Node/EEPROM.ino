#include <Arduino.h>

void EEPread(int addr, int len, char data[]) {
  char r;
  byte i = 0;
  memset(data, '\0', sizeof(data));  // Emptying the char array data

  // Reading the EEPROM bytes
  while (i < len) {
    r = EEPROM.read(addr + i);
    if (r != NULL) data[i] = r;  // Append the char array data
    i++;
  }
}

void EEPwrite(int addr, char data[]) {
  int Elength = strlen(data);

  // Writing the EEPROM bytes
  for (int i = 0; i < Elength; i++) {
    EEPROM.update(addr + i, data[i]);
  }
  EEPROM.update(Elength + 1, '\0');
}
