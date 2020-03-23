#include <Wire.h>
#include <MechaQMC5883.h>

MechaQMC5883 magneto;

float G, G_idle, G_park;
int trigger_park = 200;


bool flag_park, flag_check;

// Timer
long timer_read = 1000;
long read_prev;
long timer_check = 5000;
long check_prev;

#define LEDpin 10


void setup() {
  Wire.begin();
  Serial.begin(9600);

  pinMode (LEDpin, OUTPUT);
  digitalWrite (LEDpin, LOW);

  flag_park = 0;
  Write_Gidle();

  read_prev = millis();
}

void loop() {

  // Read every 1 second
  if (millis() - read_prev > timer_read) {
    G = Sensor_Read();
    Serial.println(G);
    read_prev = millis();
  }

  // Checking the condition
  Check_Condition();

}
