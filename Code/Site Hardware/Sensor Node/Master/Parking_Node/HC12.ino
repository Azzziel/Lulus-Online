
void Magneto_Send (String Status) {
  AT (ch_send);

  // Send Making the String and send
  Data = "";
  Data += ID_rptr;
  Data += "/";
  Data += ID_sensor;
  Data += "/";
  Data += "CAR";
  Data += ":";
  Data += Status;
  Data += "/";

  RF.print(Data);
  Data = "";
}


void Batt_Send () {
  AT (ch_send);

  // Send Making the String and send
  Data = "";
  Data += ID_rptr;
  Data += "/";
  Data += ID_sensor;
  Data += "/";
  Data += "BATT";
  Data += ":";
  Data += batt_volt;
  Data += "/";

  RF.print(Data);
  Data = "";
}

void Reset_Send () {
  // Send Making the String and send
  Data = "";
  Data += ID_rptr;
  Data += "/";
  Data += ID_sensor;
  Data += "/";
  Data += "STATUS";
  Data += ":";
  Data += "RST";
  Data += "/";

  RF.print(Data);
  Data = "";
}

void RF_Listen() {
  int i = 0;

  while (RF.available()) {
    RF_message[i] = RF.read();
    i++;
  }
  if (RF_message != NULL) {
    Data_Parsing();
  }
}


void Listen_Routine() {
  AT(ch_listen);
  flag_listen = 1;
  recheck_prev = millis();
  listen_time = random(5000, 12000);

  while ((millis() - recheck_prev <= listen_time) && flag_listen == 1) {
    RF_Listen();
  }
}
