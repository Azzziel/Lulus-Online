void AT_First () {
  digitalWrite(set_Pin, LOW);
  delay (100);
  RF.print(chSend);
  delay (200);
  RF.print(Baud);
  delay (200);
  digitalWrite(set_Pin, HIGH);

  delay (500);

  // Go to Listen mode
  AT (chListen);
}

void AT (char channel[]) {
  digitalWrite(set_Pin, LOW);
  delay (100);
  RF.print(channel);
  delay (200);
  digitalWrite(set_Pin, HIGH);
}
