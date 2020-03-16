void EEP_Read (int addr, int len, char data[]) {
  String Output = "";
  char r;
  int i = 0;
  strcpy (data, "");    // Emptying the char array data

  while (i < len) {
    r = EEPROM.read (addr + i);
    if (r != NULL)
      strcat (data, r); // Append the char array data
    i++;
  }
}

void EEP_Write (int addr, char data[]) {
  //  char c_data[20];
  int E_length = strlen(data);
//  data.toCharArray(c_data, E_length + 1);

  for (int i = 0 ; i < E_length ; i++) {
    EEPROM.update (addr + i , data [i]);
  }
  EEPROM.update (E_length + 1, '\0');
}
