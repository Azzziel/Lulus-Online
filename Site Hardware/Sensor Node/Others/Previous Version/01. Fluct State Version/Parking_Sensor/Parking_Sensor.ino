#include <Wire.h>
#include <MechaQMC5883.h>

MechaQMC5883 magneto;

int x, y, z;
float G, G_idle, G_park, G_prev;
int trigger_park = 5000;
int trigger_fluct = 1000;
int counting_fluct;

bool flag_idle, flag_park, flag_fluct;

// Timer
long timer_countfl = 5000;
long countfl_prev;

#define LEDpin 10

void setup() {
  Wire.begin();
  Serial.begin(9600);
  magneto.init();
  magneto.setMode(Mode_Continuous, ODR_10Hz, RNG_2G, OSR_256);

  flag_idle = 1;
  Sensor_Read();

  pinMode (LEDpin, OUTPUT);
  digitalWrite (LEDpin, LOW);

}

void loop() {

  // Sensor Reading Routine
  Sensor_Read();

  // Indicate a Fluctuation
  Confirm_Fluct();

  // Checking whether there is a parking state change
  Check_Parking();


  //

}
