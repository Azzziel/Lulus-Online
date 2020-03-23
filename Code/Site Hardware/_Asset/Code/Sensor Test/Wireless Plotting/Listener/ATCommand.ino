void AT () {
  digitalWrite(setPin, LOW);
  delay (100);
  //  HC12.print(Ch);
  delay (200);
  HC12.print(Baud);
  delay (200);
  digitalWrite(setPin, HIGH);
}
