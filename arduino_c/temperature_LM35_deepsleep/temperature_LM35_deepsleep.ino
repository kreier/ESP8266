/* read voltage from analog pin and export temperature */

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

int sensorValue = 0;  // value read from the LM35 10mV/°C
float temperature = 0;

void setup() {
  Serial.begin(115200);
  sensorValue = analogRead(analogInPin);
  temperature = sensorValue * 0.307 - 2.6;

  Serial.println(" ");
  Serial.print("Sensor = ");
  Serial.print(sensorValue);
  Serial.print("   Temperature = ");
  Serial.print(temperature);
  Serial.println("°C");

  delay(2000);
  ESP.deepSleep(28e6);
}

void loop() {

}
