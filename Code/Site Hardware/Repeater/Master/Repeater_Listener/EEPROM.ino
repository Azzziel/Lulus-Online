void EEP_Read (int addr, int len, char data[], byte dataLen)
char r;
int i = 0;
memset (data, '\0', dataLen);    // Emptying the char array data

while (i < len) {
  r = EEPROM.read (addr + i);
  if (r != NULL)
    data[i] = r; // Append the char array data
  i++;
}
}

void EEP_Write (int addr, char data[]) {
  int E_length = strlen(data);

  for (int i = 0 ; i < E_length ; i++) {
    EEPROM.update (addr + i , data [i]);
  }
  EEPROM.update (E_length + 1, '\0');
}
