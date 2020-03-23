//------------ Condition State ------------

void Check_Condition() {
  ////// Idle -> Parking
  if ((abs (G - G_idle) > trigger_park) && flag_park == 0) {

    // Setting the timer
    if (flag_check == 0) {
      Serial.println ("menarik nih");
      flag_check = 1;
      check_prev = millis();
    }

    // Confirm parking!
    if ((millis() - check_prev > timer_check) && ((abs (G - G_idle) > trigger_park))) {
      flag_park = 1;
      flag_check = 0;

      Serial.println ("nah ada mobil");
      digitalWrite (LEDpin, HIGH);

      Write_Gpark();
    }
  }

  ////// Parking -> Idle
  if ((abs (G - G_park) > trigger_park) && flag_park == 1) {

    // Setting the timer
    if (flag_check == 0) {
      Serial.println ("beneran pergi?");
      flag_check = 1;
      check_prev = millis();
    }

    // Confirm leaving!
    if ((millis() - check_prev > timer_check) && ((abs (G - G_park) > trigger_park))) {

      Serial.println ("mobilnya keluar");
      digitalWrite (LEDpin, LOW);

      flag_park = 0;
      flag_check = 0;
      Write_Gidle();
    }
  }
}


//// Writing the base G value
// G idle
void Write_Gidle() {
  G_idle = Sensor_Detail_Read();
}

// G park
void Write_Gpark() {
  G_park = Sensor_Detail_Read();
}
