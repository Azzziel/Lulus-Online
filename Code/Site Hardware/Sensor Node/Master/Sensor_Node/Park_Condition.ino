#include <Arduino.h>

//------------ Condition State ------------

void checkParkCondition() {
  //=========================
  //-------- Idle -> Parking
  //=========================

  if ((abs(G - Gidle) > valueTrigger) && !canPark) {
    Serial.println("menarik nih, beneran ada?");

    // Sleep while waiting to recheck
    delay(200);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

    // Time to recheck!
    G = sensorRead();
    Serial.print("nilainya sekarang: ");
    Serial.println(G);

    // Indeed, there is a car
    if ((abs(G - Gidle) >= valueTrigger)) {
      canPark = 1;
      Serial.println("wah beneran ada mobil");

      //-------- Sending Routine
      dataSendRF("CAR", "0");  // Tell that it's occupied
      listenRoutine();
      // Wait until it get the ACK
      while (canListen) {
        dataSendRF("CAR", "0");
        listenRoutine();
      }
      //---------------------

      // Sampling for saving the based value
      writeGpark();
      Serial.print("Nilai patokan Gpark: ");
      Serial.println(Gpark);
    }
  }

  //=========================
  //-------- Parking -> Idle
  //=========================

  if ((abs(G - Gidle) < valueIgnored) && (abs(G - Gpark) > valueTrigger) &&
      canPark) {
    // Setting the timer
    Serial.println("beneran pergi?");

    // Sleep while waiting to recheck
    delay(200);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

    // Time to recheck!
    G = sensorRead();
    Serial.print("nilainya sekarang: ");
    Serial.println(G);

    // Indeed, its' gone
    if ((abs(G - Gidle) < valueIgnored) && (abs(G - Gpark) > valueTrigger)) {
      canPark = 0;
      Serial.println("mobilnya keluar");

      // Sending Routine
      dataSendRF("CAR", "1");  // Tell that it's available
      listenRoutine();
      // Wait until it get the ACK
      while (canListen) {
        dataSendRF("CAR", "1");
        listenRoutine();
      }
      //---------------------

      // Sampling for based value
      writeGidle();
      Serial.print("Nilai patokan Gidle: ");
      Serial.println(Gidle);
    }
  }
}

//=========================
//-------- Write the G base
//=========================

// G idle
void writeGidle() {
  Gidle = sensorReadDetail();

  // Compare to the G idle EEPROM
  GidleEEPnum = atoi(GidleEEP);

  // If its premade value ('99')
  if (GidleEEPnum == 99) {
    sprintf(GidleEEP, "%d",
            Gidle);  // save Gidle num value to GidleEEP char value
    EEPwrite(21, GidleEEP);
  }

  // If the value similar
  else if (abs(GidleEEPnum - Gidle) < valueIgnored) {
    // Sending Routine
    dataSendRF("CAR", "1");  // Tell that it's available
    listenRoutine();
    // Wait until it get the ACK
    while (canListen) {
      dataSendRF("CAR", "1");
      listenRoutine();
    }
  }

  // If the value difference significantly
  else if (abs(GidleEEPnum - Gidle) > valueTrigger) {
    // Sending Routine
    dataSendRF("CAR", "0");  // Tell that it's occupied
    listenRoutine();
    // Wait until it get the ACK
    while (canListen) {
      dataSendRF("CAR", "0");
      listenRoutine();
    }
  }

  // If the value difference considerable, update the EEPROM, probably due to
  // the nature or sensor in the long run
  else if ((abs(GidleEEPnum - Gidle) > valueIgnored) &&
           (abs(GidleEEPnum - Gidle) < valueConsideredEEP)) {
    // Sending Routine
    dataSendRF("CAR", "1");  // Tell that it's available
    listenRoutine();
    // Wait until it get the ACK
    while (canListen) {
      dataSendRF("CAR", "1");
      listenRoutine();
    }

    // Update the EEPROM
    sprintf(GidleEEP, "%d",
            Gidle);  // save Gidle num value to GidleEEP char value
    EEPwrite(21, GidleEEP);
  }
}

// G park
void writeGpark() { Gpark = sensorReadDetail(); }
