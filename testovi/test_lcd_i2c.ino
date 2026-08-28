#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Za Wokwi koristi 0x27.
// Ako fizicki LCD ima drugu adresu, promijeni 0x27 u adresu koju nadje I2C scanner.
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("LCD RADI!");

  lcd.setCursor(0, 1);
  lcd.print("Arduino Nano");
}

void loop() {
}
