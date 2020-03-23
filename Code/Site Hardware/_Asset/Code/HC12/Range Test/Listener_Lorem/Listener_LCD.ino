#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin
LiquidCrystal_I2C lcd(0x27, 16, 2);



void setup() {
//  Serial.begin(9600);             // Serial port to computer
  HC12.begin(2400);               // Serial port to HC12

  lcd.init();
  lcd.backlight();

  //  lcd.setCursor(0, 0);
  lcd.print("Range Test!");
}

void loop() {
  if (HC12.available()) {
    String reading = HC12.readStringUntil('\n');

    String reading1;
    String reading2;

    unsigned int readCount = 2;
    String *readings[readCount]{&reading1, &reading2};

    unsigned int j = 0;
    for (unsigned int i = 0; i < reading.length(); ++i) {
      if (j >= readCount) {
        break;
      }
      
      if (reading[i] != '_') {
        *readings[j] += reading[i];
      } else {
        ++j;
      }
    }

//    Serial.println(*readings[0]);
//    Serial.println(*readings[1]);
//    Serial.println();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(*readings[0]);
    lcd.setCursor(0, 1);
    lcd.print(*readings[1]);
  }
}
