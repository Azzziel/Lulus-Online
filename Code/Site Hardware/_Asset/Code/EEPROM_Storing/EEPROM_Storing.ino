#include <EEPROM.h>

// ID Setting
char myID[20];
char pairID[20];

long myID_hex = 0x4f23;
long pairID_hex = 0x5b5b;

// Address in EEPROM
int Addr_myID = 1;
int Addr_pairID = 21;

void setup() {
  Serial.begin (9600);

  // Convert hex -> C string
  sprintf(myID, "%ld", myID_hex);
  sprintf(pairID, "%ld", pairID_hex);

  // Writing Data
  //  clear_eeprom (0, 50);
  //  EEPROMwriteString (Addr_myID, myID);
  //  EEPROMwriteString (Addr_pairID, pairID);

  Serial.println ("Reading");
  String Reading_myID;
  String Reading_pairID;

  Reading_myID  = EEPROMreadString(Addr_myID) ;
  Reading_pairID  = EEPROMreadString(Addr_pairID) ;

  Serial.print ("my ID: ");
  Serial.print (Reading_myID);
  Serial.print ("   | pair ID: ");
  Serial.println (Reading_pairID);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void EEPROMwriteString (int addr, char data[]) {
  int E_length = strlen(data);

  // Writing the EEPROM bytes
  for (int i = 0 ; i < E_length ; i++) {
    EEPROM.write (addr + i , data [i]);
  }
  EEPROM.write (E_length + 1, '\0');
}


String EEPROMreadString (int addr) {
  String Output = "";
  char r;
  int i = 0;

  while (i < 20) {
    r = EEPROM.read (addr + i);
    Output += r;
    i++;
  }
  return Output;
}



void clear_eeprom (int start_bit, int end_bit) {
  for (start_bit; start_bit < end_bit ; start_bit++) {
    EEPROM.write(start_bit, '\0');
    yield();
  }
}
