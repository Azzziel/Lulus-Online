void battRoutine() {
  if (battCount == battTrigger) {

    float total, voltageValue;
    int avgNum = 100;

    for (int avgPos = 0 ; avgPos < avgNum ; avgPos++) {
      voltageValue = analogRead(batt_Pin) * 3.3 / 1024;
      total += voltageValue;
      delay (5) ;
    }
    float avgVolt =  total / avgNum;

    // Calculate the battery volt using equation
    // y = 1.3235x
    float battVolt = 1.3235 * avgVolt;

    // Giving the indicator value based on current volt
    // 4 [> 3.75] ; 3 [3.75 - 3.6] ; 2 [3.6 - 3.35] ; 1 [< 3.35]
    int battIndicator;
    if (battVolt > 3.75) {
      battIndicator = 4;
    }
    else if (battVolt <= 3.75 && battVolt > 3.6) {
      battIndicator = 3;
    }
    else if (battVolt <= 3.6 && battVolt > 3.35) {
      battIndicator = 2;
    }
    else if (battVolt <= 3.35) {
      battIndicator = 1;
    }

    // Sending Routine
    dataSendRF("BATT", battIndicator);
    listenRoutine();
    // Wait until it get the ACK
    while (canListen) {
      dataSendRF("BATT", battIndicator);
      listenRoutine();
    }
    battCount = 0;
  }

  else {
    battCount++;
  }
}
