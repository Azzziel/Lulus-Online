void AT_first () {
  digitalWrite(set_Pin, LOW);
  delay (100);
  RF.print(ch_send);
  delay (200);
  RF.print(Baud);
  delay (200);
  digitalWrite(set_Pin, HIGH);

  // Sending the Reset Notification to Repeater
  Reset_Send ();
  delay (500);

  // Go to Listen mode
  AT (String(ch_listen));
}

void AT (String channel) {
  digitalWrite(set_Pin, LOW);
  delay (100);
  RF.print(channel);
  delay (200);
  digitalWrite(set_Pin, HIGH);
}

void AT_sleep () {
  digitalWrite(set_Pin, LOW);
  delay (100);
  RF.print("AT+SLEEP");
  delay (200);
  digitalWrite(set_Pin, HIGH);
}
