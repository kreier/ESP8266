// Test datatypes

void setup() {
  Serial.begin(74880);
  uint32_t test = 4294967295;
  float test2 = test;
  float test3 = test / 100.0;
  Serial.print("\n\nTest.\n");
  Serial.println(test);
  Serial.println(test2);
  Serial.println(test3);
  Serial.println(test / test3);
}

void loop() {
  delay(1000);
}

