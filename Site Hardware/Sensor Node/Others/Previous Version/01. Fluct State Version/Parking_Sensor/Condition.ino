//------------ Condition State ------------

//--------------- Park ---------------------------------

// Checking whether there is a parking state change
void Check_Parking() {

  // Idle -> Parking
  if ((abs (G - G_idle) > trigger_park) && flag_idle == 1 && flag_fluct == 1) {
    flag_idle = 0;
    flag_park = 1;
    flag_fluct = 0;

    Serial.println ("nah ada mobil");
    digitalWrite (LEDpin, HIGH);
  }

  // Parking -> Idle
  if ((abs (G - G_park) > trigger_park) && flag_idle == 0 && flag_fluct == 1) {
    flag_idle = 1;
    flag_park = 0;
    flag_fluct = 0;

    Serial.println ("mobilnya pergi");
    digitalWrite (LEDpin, LOW);
  }
}

// Setting a G value based on current state
void Gset_Value() {
  // Making a G-Idle Value
  // In idle state and the G reading is stable
  if (flag_idle == 1 && flag_fluct == 0) {
    G_idle = G;
  }

  // Making a G-Park Value
  // The car already detected and the G reading already stable
  if (flag_park == 1 && flag_fluct == 0) {
    G_park = G;
  }
}

// Parking -> Idle


//--------------- Fluct ---------------------------------
// Indicate a Fluctuation
void Confirm_Fluct() {
  // Fluct 0 -> 1
  if (counting_fluct >= 5 && flag_fluct == 0) {
    flag_fluct = 1;
    counting_fluct = 0;

    Serial.println ("fluct 0 ke 1!");
  }

  // Fluct 1 -> 0
  if (flag_fluct == 1 && (abs (G_prev - G) <= trigger_fluct)) {
    flag_fluct = 0;

    Serial.println ("fluct 1 ke 0");
  }
}


// Turning down false alarm (counting fluct)
void False_Fluct() {
  if (((millis () - countfl_prev) > timer_countfl) && counting_fluct >= 1) {
    counting_fluct = 0;

    Serial.println ("abaikan kenaikannya");
  }
}
