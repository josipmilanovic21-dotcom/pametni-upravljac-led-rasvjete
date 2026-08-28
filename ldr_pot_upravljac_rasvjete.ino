const int ldrPin = A0;
const int potPin = A1;
const int ledPin = 9;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  int refValue = analogRead(potPin);

  int error = refValue - ldrValue;
  int ledValue;

  if (error > 0) {
    ledValue = map(error, 0, 1023, 0, 255);
  } else {
    ledValue = 0;
  }

  ledValue = constrain(ledValue, 0, 255);

  analogWrite(ledPin, ledValue);

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print("  REF: ");
  Serial.print(refValue);
  Serial.print("  LED: ");
  Serial.println(ledValue);

  delay(300);
}
