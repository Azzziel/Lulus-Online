//------------ Condition State ------------

void Check_Condition() {
  ////// Idle -> Parking
  if ((abs (G - G_idle) > trigger_park) && flag_park == 0) {

    // Setting the timer
    if (flag_recheck == 0) {
      Serial.println ("menarik nih, beneran ada?");
      flag_recheck = 1;
      flag_check = 0;

      recheck_prev = millis();
    }

    // Time to recheck!
    if ((millis() - recheck_prev > timer_recheck) && flag_recheck == 1) {
      G = Sensor_Read();

      // Indeed, there is a car
      if (((abs (G - G_idle) >= trigger_park))) {
        flag_park = 1;
        flag_check = 1;
        flag_recheck = 0;

        Serial.println ("wah beneran ada mobil");

        //-------- HC12 Work
        Magneto_Send ("1");
        delay (500);
        Listen_Routine();

        // If the sensor still doesn't get the acknowledge
        while (flag_listen == 1) {
          Magneto_Send ("1");
          Listen_Routine();
        }
        //---------------------

        //Sampling for based value
        Write_Gpark();
      }

      // You got a wrong signal
      else {
        flag_check = 1;
        flag_recheck = 0;
      }
    }
  }


  ////// Parking -> Idle
  if ((abs (G - G_idle) > trigger_park) && flag_park == 1) {

    // Setting the timer
    if (flag_recheck == 0) {
      Serial.println ("beneran pergi?");
      flag_recheck = 1;
      flag_check = 0;

      recheck_prev = millis();
    }

    // Time to recheck!
    if ((millis() - recheck_prev > timer_recheck) && flag_recheck == 1) {
      G = Sensor_Read();

      // Indeed, its' gone
      if (((abs (G - G_idle) >= trigger_park))) {
        flag_park = 1;
        flag_check = 1;
        flag_recheck = 0;

        Serial.println ("mobilnya keluar");

        //HC12 Work
        Magneto_Send ("0");
        delay (500);
        Listen_Routine();

        // If the sensor still doesn't get the acknowledge
        while (flag_listen == 1) {
          Magneto_Send ("1");
          Listen_Routine();
        }
        //---------------------

        //Sampling for based value
        Write_Gidle();
      }
      // You got a wrong signal
      else {
        flag_check = 1;
        flag_recheck = 0;
      }
    }
  }
}

//// Writing the base G value
// G idle
void Write_Gidle() {
  G_idle = Sensor_Detailed();
}

// G park
void Write_Gpark() {
  G_park = Sensor_Detailed();
}
