const int ldrPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);

  Serial.print("LDR: ");
  Serial.println(ldrValue);

  delay(300);
}
