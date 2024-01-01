// Wemos ESP8266 with battery holder and 0.96" 128x64 OLED display
// one button as input 2024/01/01

#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <time.h>
#include <math.h>
#include <ESP_EEPROM.h>

U8G2_SH1106_128X64_NONAME_F_SW_I2C  u8g2(U8G2_R0, 12, 14); // wide Lolin ESP8266 board
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 4, 5);   // integrated board

#define FONT7     u8g2_font_5x7_mf
// #define FONT7     u8g2_font_resoledmedium_tr
#define FONT10    u8g2_font_6x10_mf
#define FONT15    u8g2_font_9x15B_mf
#define PROFONT15 u8g2_font_profont15_mf
#define PROFONT17 u8g2_font_profont17_mf

int buttonDOWN = 12;
int buttonUP   = 13;
int buttonOK   = 0;           // 14 for shield
int led        = LED_BUILTIN; // LED_BUILTIN

double start;
double timer;
int  column    = 10;
long found     = 4;   // we already know 2, 3, 5, 7
int  divisors  = found;
uint16_t primes[6550]  = {3, 5, 7}; // prime #671 is 5009 > sqrt(25 million)
const uint32_t scope[] = {100, 1000, 10000, 100000, 1000000, 10000000, 25000000, 100000000, 1000000000, 2147483647, 4294967295};
const long reference[] = {25, 168, 1229, 9592, 78498, 664579, 1565927, 5761455, 50847534, 105097564, 203280221};
const char* label[]    = {"100", "1,000", "10,000", "100,000", "1,000,000", "10,000,000", "25,000,000", "100,000,000", "1,000,000,000", "2,147,483,647", "4,294,967,295"};

int is_prime(long number) {
  int prime = 1;
  for (long divider = 3; divider < (long)(sqrt(number)) + 1; divider += 2) {
    if (number % divider == 0) {
      prime = 0;
      break;
    }
  }
  return prime;
}

int find_primes(long limit) {
  int column = 0;
  Serial.print("Finding primes to limit of ");
  Serial.println(limit);
  for(long number=11; number < limit + 1; number += 2) { 
    if( is_prime(number) == 1) {
      primes[found - 1] = number;
      found++;
    }
    // if(number > 21700) {
    if((number + 1) % 100 == 0) {
      Serial.print(" ");
      Serial.print(number);
      delay(1);
      // Serial.print(".");
      column += 1;
      if(column > 15) {
        column = 0;
        Serial.print("\n");
      }
    }
  }
  primes[found - 1] = limit;
  divisors = found - 1;
  Serial.print("\n");
  return 1;
}

int is_prime_fast(uint32_t number) {
  long largest_divider = (long)(sqrt(number));
  int flag_prime = 1;
  for(int i=0; i < divisors; i++) {
    if(number % primes[i] == 0) {
      flag_prime = 0;
      break;
    }
    if(primes[i] > largest_divider) {
      break;
    }
  }
  return flag_prime;
}

void elapsed_time(long seconds) {
  int hours = (int)seconds/3600;
  int minutes = (int)(seconds/60 - hours*60);
  int sec = (int)(seconds - minutes*60 - hours*3600);
  Serial.print(" ");
  Serial.print(hours);
  Serial.print("h ");
  Serial.print(minutes);
  Serial.print("min ");
  Serial.print(sec);
  Serial.print("s ");
  // now output on the oled display
  // u8g2.print(hours);
  // u8g2.print("h ");
  // u8g2.print(minutes);
  // u8g2.print("min ");
  // u8g2.print(sec);
  // u8g2.print("s ");
}

void show_last_results(int startvalue) {
  Serial.print("\nPrevious results ESP8266 12E:\n");
  Serial.print("    last        seconds   \n");
  u8g2.clearBuffer();
  u8g2.setFont(FONT7);
  u8g2.drawStr(0, 8, "Previous runs ESP8266 12E");
  u8g2.drawStr(0, 16, "     last     seconds");
  int digits = 6;
  for(int i = 0; i < 6; i++) {
    int spaces = 13 - strlen(label[i + startvalue]);
    for(int j = 0; j < spaces; j++) {
      Serial.print(" ");
    }
    u8g2.setCursor(spaces * 5, 24 + 8 * i);
    u8g2.print(label[i + startvalue]);
    u8g2.print("  ");
    Serial.print(label[i + startvalue]);
    Serial.print("    ");
    float last_time;
    EEPROM.get((i  + startvalue) * 4 + 1, last_time);
    Serial.print(last_time, 6);
    if(last_time > 9.9) {
      digits = 6 - (int)log10(last_time);
    }
    else {
      digits = 6;
    }
    u8g2.print(String(last_time, digits));
    Serial.print("\n");
  }  
}

void show_last(int index) {
  int prespace = 9 - (int)log10(scope[index]);
  u8g2.setCursor(0, 16);
  u8g2.setFont(u8g2_font_profont22_tn);    
  for(int i = 0; i < prespace; i++) { u8g2.print(" "); }
  u8g2.print(scope[index]);
}

void show_scope(int index) {
  u8g2.setFont(PROFONT17);
  u8g2.drawStr(20, 16, "Primes to");
  u8g2.setCursor(5, 32);
  u8g2.print(label[index]);
  u8g2.print("           ");
}

void setup() {
  Serial.begin(74880);
  u8g2.begin();
  u8g2.clearBuffer();
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(buttonOK  , INPUT_PULLUP);
  // pinMode(buttonUP  , INPUT_PULLUP);
  // pinMode(buttonDOWN, INPUT_PULLUP);

  // delay start by 2 seconds with animation and serial output
  u8g2.setFont(PROFONT17);
  u8g2.drawStr(20, 20, "Primes");
  for (int i = 0; i < 2; i++) {
    Serial.print(".");
    u8g2.drawStr(80+10*i, 20, ".");
    u8g2.sendBuffer();
    delay(1000);
  }
  digitalWrite(led, HIGH);

  // previous run
  EEPROM.begin(48);
  Serial.print("\n\nEEPROM already used to ");
  Serial.print(EEPROM.percentUsed());
  Serial.print("%\n");
  int current_index = 0;
  show_last_results(current_index);
  u8g2.sendBuffer();
  timer = 0;
  do {
    if (digitalRead(buttonOK) == 0) {
      current_index += 1;
      if(current_index > 5) {
        current_index = 0;
      }
      start = millis();
      show_last_results(current_index);
      while(digitalRead(buttonOK) == 0) {
        delay(10);
      }
      u8g2.sendBuffer();
      timer = millis() - start;
    }
    delay(10);
  }  while (timer < 500);


  // select the range for the new calculation *****************
  current_index = 0;
  u8g2.clearBuffer();
  show_scope(current_index);
  u8g2.sendBuffer();
  timer = 0;
  do {
    if (digitalRead(buttonOK) == 0) {
      current_index++;
      if(current_index > 10) {
        current_index = 0;
      }
      start = millis();
      show_scope(current_index);
      while(digitalRead(buttonOK) == 0) {
        delay(10);
      }
      u8g2.sendBuffer();
      timer = millis() - start;
    }
    delay(10);
  }  while (timer < 500);
  current_index--;
  if (current_index < 0) { current_index = 10; }
  show_scope(current_index);
  u8g2.sendBuffer();

  // start calculating in micros for higher precision for the first 7 calculations
  long last = scope[current_index];
  found = 4;   // we already know 2, 3, 5, 7
  Serial.println("\n\nPrime v5.3");
  Serial.print("Primes until ");
  Serial.println(label[current_index]);
  u8g2.setFont(FONT10);
  u8g2.drawStr(15, 50, "Now calculating");
  u8g2.drawStr(15, 62, "prime factors.");
  u8g2.sendBuffer();
  float last_percent = (float)last / 100.0;
  
  start = micros();      
  uint16_t largest_divider = (sqrt(last)); 
  if(largest_divider % 2 == 0)
  {
    largest_divider += 1;
  }
  find_primes(largest_divider);
  Serial.print("Found ");
  Serial.print(found);
  Serial.print(" primes until ");
  Serial.print(largest_divider);
  Serial.print(" to use as divisors.\n");
  if (last > 100000) { delay(100); }
  u8g2.clearBuffer();
  show_scope(current_index);
  u8g2.sendBuffer();  
  long dot = millis();
  int column = 0;
  float numberf = 0.0;
  for(uint32_t number = largest_divider + 2; number < last; number += 2)
  {
    found += is_prime_fast(number);
    if ((millis() - dot) > 1234) {
      Serial.print(".");
      dot = millis();
      numberf = (float)number;
      Serial.print(last_percent);
      Serial.print("  ");
      Serial.print(numberf);
      Serial.print("  ");
      Serial.println(number / last_percent);
      column += 1;
      if(column % 2 == 0) {
        digitalWrite(led, HIGH);
      }
      else
      {
        digitalWrite(led, LOW);
      }
      u8g2.setFont(FONT10);
      u8g2.setCursor(0, 44);
      u8g2.print(number);
      u8g2.print(" - ");
      u8g2.print(numberf / last_percent);
      u8g2.println("%   ");
      timer = (micros() - start)/1000000;
      u8g2.setCursor(0, 54);
      u8g2.print(String(timer, 3));
      u8g2.println(" seconds ");
      int hours = (int)timer/3600;
      int minutes = (int)(timer/60 - hours*60);
      u8g2.setCursor(0, 64);
      u8g2.print(hours);
      u8g2.print("h ");
      u8g2.print(minutes);
      u8g2.print("min ");
      u8g2.sendBuffer();
      if(column > 40) {
        column = 0;
        elapsed_time(dot/1000);
        Serial.print(" - ");
        Serial.print(number);
        Serial.print(" ");
        Serial.print(numberf / last_percent);
        Serial.print("% \n");
      }
    }
  }
  const float duration = (micros() - start)/1000000;
  if(duration > 2) {
    Serial.print("\n");
  }    
  Serial.print("Found ");
  Serial.print(found);
  Serial.print(" prime numbers. It should be ");
  Serial.print(reference[current_index]);
  Serial.print(".\nThis took ");
  Serial.print(duration, 3);
  Serial.print(" seconds. ");
  elapsed_time(duration);
  EEPROM.put(current_index * 4 + 1, duration);
  boolean ok1 = EEPROM.commit();
  Serial.println((ok1) ? "- OK" : "Commit failed");
  Serial.print("\n");
}

void loop() {
  Serial.print(".");
  delay(5000);
}

