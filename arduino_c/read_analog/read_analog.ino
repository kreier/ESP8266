/* read analog pin and put out*/

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

int sensorValue = 0;  // value read from the pot

void setup() {
  Serial.begin(115200);
}

void loop() {
  sensorValue = analogRead(analogInPin);
  
  Serial.print("sensor = ");
  Serial.println(sensorValue);

  delay(1000);
}
