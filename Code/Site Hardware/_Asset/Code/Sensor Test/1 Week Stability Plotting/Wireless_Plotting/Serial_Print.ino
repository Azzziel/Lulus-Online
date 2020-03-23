void SerPrint () {
  Serial.print("G : ");
  Serial.print(G);
  Serial.println();
  Serial.print("Temp : ");
  Serial.print(tmp);
  Serial.print("      | Hum : ");
  Serial.print(hmd);
  Serial.println();
}
