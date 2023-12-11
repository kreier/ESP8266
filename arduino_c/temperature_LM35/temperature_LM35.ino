/* read voltage from analog pin and export temperature */

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

int sensorValue = 0;  // value read from the LM35 10mV/°C
float temperature = 0;

void setup() {
  Serial.begin(74880);
}

void loop() {
  sensorValue = analogRead(analogInPin);
  temperature = sensorValue * 0.307;
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println("°C");
  delay(2000);  
}
