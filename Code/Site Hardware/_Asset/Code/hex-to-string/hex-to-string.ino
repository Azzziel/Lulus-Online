char data[50];
long angka = 0xFFFDFF;


void setup() {
  Serial.begin(9600);

  Serial.println(angka);
  sprintf(data, "%ld", angka);
  Serial.println(data);

}

void loop() {
  // put your main code here, to run repeatedly:

}
