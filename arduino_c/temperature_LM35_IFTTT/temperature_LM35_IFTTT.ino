/* read voltage from analog pin and upload via IFTTT Webhooks to Google Sheet */

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <credentials_phumy.h>

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
const int ledPin = 2;

int sensorValue = 0;    // value read from the LM35 10mV/°C
float temperature = 0;

const char* server = "maker.ifttt.com";
// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 2 minutes = 120 seconds
uint64_t TIME_TO_SLEEP = 120;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  
  Serial.begin(115200);
  delay(50);
  getTemperature();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println("°C");  
  digitalWrite(ledPin, HIGH);
  delay(1000);  
  digitalWrite(ledPin, LOW);  
  initWifi();
  makeIFTTTRequest();
  digitalWrite(ledPin, HIGH);
  Serial.println("Going to sleep now for 2 minutes."); 
  ESP.deepSleep(120e6);
}

void loop() {
}

// Establish a Wi-Fi connection with your router
void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  String jsonObject = String("{\"value1\":\"") + sensorValue
                          + "\",\"value2\":\"" + temperature
                          + "\",\"value3\":\"" + millis()
                          + "\"}";
                      
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}

void getTemperature() {
  sensorValue = analogRead(analogInPin);
  temperature = sensorValue * 0.307;  
}
