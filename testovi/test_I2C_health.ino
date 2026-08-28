#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);

  Serial.println("I2C scanner start");
}

void loop() {
  byte error, address;
  int devices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C uredjaj nadjen na adresi 0x");

      if (address < 16) {
        Serial.print("0");
      }

      Serial.println(address, HEX);
      devices++;
    }
  }

  if (devices == 0) {
    Serial.println("Nema I2C uredjaja.");
  }

  Serial.println();
  delay(3000);
}
