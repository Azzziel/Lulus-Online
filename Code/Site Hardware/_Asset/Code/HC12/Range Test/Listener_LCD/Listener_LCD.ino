#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin
LiquidCrystal_I2C lcd(0x27, 16, 2);

String Message;

void setup() {
  //  Serial.begin(9600);             // Serial port to computer
  HC12.begin(2400);               // Serial port to HC12

  lcd.init();
  lcd.backlight();

  //  lcd.setCursor(0, 0);
  lcd.print("Range Test!");
}

void loop() {
  while (HC12.available()) k{
    Message = HC12.readString();
 //       Serial.println(Message);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Message);
  }
}
