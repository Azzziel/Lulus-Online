void Batt_Routine() {
  if (batt_count == batt_max) {

    float Total;
    int arrayPos;
    int arrayNum = 50;
    float VoltageValue;

    for (arrayPos = 0 ; arrayPos < arrayNum ; arrayPos++) {
      VoltageValue = analogRead(batt_Pin) * 5.0 / 1024;
      Total += VoltageValue ;
      delay (5) ;
    }
    batt_volt = Total / arrayNum;
    batt_count = 0;

    Batt_Send ();
    Listen_Routine();

    // If the sensor still doesn't get the acknowledge
    while (flag_listen == 1) {
      Magneto_Send ("1");
      Listen_Routine();
    }
  }

  else {
    batt_count++;
  }
}
