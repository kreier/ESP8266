// Wemos ESP8266 with battery holder and 0.96" 128x64 OLED display
// three input buttones, no schematics

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int buttonPins[] = {12, 13, 14, 15, 16};
int buttonState = 0;

void setup() {
  Serial.begin(74880);
  Wire.begin(5, 4);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  for(int i = 0; i < 5; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  display.clearDisplay();
  display.setRotation(2); // 180 degrees
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);
  display.display();  
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  for(int i = 0; i < 5; i++) {
    buttonState = digitalRead(buttonPins[i]);
    display.print("Pin ");
    display.print(buttonPins[i]);
    display.print(" - value: ");
    display.println(buttonState);
    display.display();  
  }  
  delay(1000);
}

void draw_hello_world() {
  display.clearDisplay();
  display.setRotation(2); // 180 degrees
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.println(F("Helloworld"));
  display.display();
  Serial.println("\nHello, world!");
}

