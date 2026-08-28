#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// U Wokwi simulaciji LCD najcesce koristi adresu 0x27.
// Ako se koristi fizicki LCD, adresu treba provjeriti pomocu I2C scannera.
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ldrPin = A0;   // LDR senzor
const int potPin = A1;   // potenciometar
const int ledPin = 9;    // LED preko otpornika na D9

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Pametni LED");
  lcd.setCursor(0, 1);
  lcd.print("Pokretanje...");
  delay(1500);

  lcd.clear();
}

void loop() {
  int ldrValue = analogRead(ldrPin);   // stvarna razina svjetla
  int refValue = analogRead(potPin);   // zadana vrijednost

  int error = refValue - ldrValue;
  int ledValue;

  if (error > 0) {
    ledValue = map(error, 0, 1023, 0, 255);
  } else {
    ledValue = 0;
  }

  ledValue = constrain(ledValue, 0, 255);

  // Upravljanje LED-icom pomocu PWM-a
  analogWrite(ledPin, ledValue);

  // Serial Monitor prikaz
  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print("  REF: ");
  Serial.print(refValue);
  Serial.print("  LED: ");
  Serial.println(ledValue);

  // LCD prvi red
  lcd.setCursor(0, 0);
  lcd.print("L:");
  lcd.print(ldrValue);
  lcd.print(" R:");
  lcd.print(refValue);
  lcd.print("   ");

  // LCD drugi red
  lcd.setCursor(0, 1);
  lcd.print("LED:");
  lcd.print(ledValue);
  lcd.print(" PWM     ");

  delay(300);
}
