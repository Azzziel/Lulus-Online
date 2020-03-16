const byte numChars = 20;
char receiveRF[numChars];
int i = 0;

void setup() {
  Serial.begin(9600);

}

void loop() {
  while (Serial.available()) {
    char bufferkata = Serial.read();
    if (bufferkata == (char)0x0A) {
      readData(receiveRF);
      break;
    }
    else {
      receiveRF[i] = bufferkata;
      i++;
    }
  }
}

void readData(char receiveRF[]) {
  if (strcmp(receiveRF, '\0')) {
    Serial.println(receiveRF);

    if (strcmp(receiveRF, "bucin") == 0) {
      Serial.println ("nah loh");
    }
    i = 0;
    memset (receiveRF, '\0', numChars);

  }
}
