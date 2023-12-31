// Wemos ESP8266 with battery holder and 0.96" 128x64 OLED display
// one button as input

#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <time.h>
#include <math.h>
#include <ESP_EEPROM.h>

U8G2_SH1106_128X64_NONAME_F_SW_I2C  u8g2(0, 12, 14); // wide Lolin ESP8266 board
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(0, 4, 5);   // integrated board

int buttonUP   = 13;
int buttonDOWN = 12;
int buttonOK   = 0;    // 14 for shield

double start;
double timer;
int column = 10;
long found = 4;   // we already know 2, 3, 5, 7
int divisors = found;
uint16_t primes[6542] = {3, 5, 7}; // prime #671 is 5009 > sqrt(25 million)
int led = LED_BUILTIN; // LED_BUILTIN
const uint32_t scope[] = {100, 1000, 10000, 100000, 1000000, 10000000, 25000000, 100000000, 1000000000, 2147483647, 4294967295};
const long reference[] = {25, 168, 1229, 9592, 78498, 664579, 1565927, 5761455, 50847534, 105097564, 203280221};

int is_prime(long number) {
  int prime = 1;
  for (long divider = 3; divider < (long)(sqrt(number)) + 1; divider += 2)
  {
    if (number % divider == 0)
    {
      prime = 0;
      break;
    }
  }
  return prime;
}

int find_primes(long limit) {
  int column = 0;
  for(long number=11; number < limit + 1; number += 2) { 
    if( is_prime(number) == 1) {
      primes[found - 1] = number;
      found++;
    }
    if(number > 21700) {
    // if((number + 1) % 100 == 0) {
      Serial.print(" ");
      Serial.print(number);
      // Serial.print(".");
      column += 1;
      if(column > 20) {
        column = 0;
        Serial.print("\n");
      }
    }
  }
  primes[found - 1] = limit;
  divisors = found - 1;
  return 1;
}

int is_prime_fast(uint32_t number) {
  long largest_divider = (long)(sqrt(number));
  int flag_prime = 1;
  for(int i=0; i < divisors; i++)
  {
    if(number % primes[i] == 0) 
    {
      flag_prime = 0;
      break;
    }
    if(primes[i] > largest_divider)
    {
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
  u8g2.print(hours);
  u8g2.print("h ");
  u8g2.print(minutes);
  u8g2.print("min ");
  u8g2.print(sec);
  u8g2.print("s ");
}

void last_results(int startvalue) {
  Serial.print("\nPrevious results ESP8266 12E:\n");
  Serial.print("    last        seconds   \n");  
  u8g2.setCursor(0, 16);
  int digits = 6;
  for(int i = startvalue; i < 6 + startvalue; i++) {
    int spaces = 9 - (int)log10(scope[i]);
    for(int j = 0; j < spaces; j++) {
      Serial.print(" ");
      u8g2.print(" ");
    }
    Serial.print(scope[i]);
    u8g2.print(scope[i]);
    Serial.print("    ");
    u8g2.print(" ");
    float last_time;
    EEPROM.get(i * 4 + 1, last_time);
    Serial.print(last_time, 6);
    if(last_time > 9.9) {
      digits = 6 - (int)log10(last_time);
    }
    else {
      digits = 6;
    }
    u8g2.print(last_time, digits);
    Serial.print("\n");
    u8g2.print("\n");
    u8g2.sendBuffer();
  }  
}

void show_last(int index) {
  int prespace = 9 - (int)log10(scope[index]);
  u8g2.setCursor(0, 16);
  u8g2.setFont(u8g2_font_profont22_tn);    
  for(int i = 0; i < prespace; i++) { u8g2.print(" "); }
  u8g2.print(scope[index]);
  u8g2.sendBuffer();  
}

void setup() {
  Serial.begin(74880);
  u8g2.begin();
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  // pinMode(buttonUP  , INPUT_PULLUP);
  // pinMode(buttonDOWN, INPUT_PULLUP);
  pinMode(buttonOK  , INPUT_PULLUP);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); // Don't proceed, loop forever
  // }
  u8g2.clearBuffer();
  // u8g2.setRotation(2); // 180 degrees
  u8g2.setFont(u8g2_font_NokiaSmallPlain_tf);
  // u8g2.setTextColor(SSD1306_WHITE, SSD1306_BLACK); // Draw white text
  u8g2.setCursor(0, 0);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,24,"Hello World!");
  } while ( u8g2.nextPage() );
  for (int i = 0; i < 2; i++) {
    Serial.print(".");
    u8g2.print(".");
    u8g2.sendBuffer();
    delay(1000);
  }
  digitalWrite(led, HIGH);  
  u8g2.clearBuffer();
  u8g2.setCursor(0, 0);

  // previous run
  EEPROM.begin(48);
  Serial.print("\n\nEEPROM already used to ");
  Serial.print(EEPROM.percentUsed());
  Serial.print("%\n");
  u8g2.print("Previous ESP8266 12E\n");
  u8g2.print("    last    seconds\n");
  int current_value = 0;
  last_results(current_value);
  while(digitalRead(buttonOK) == 1) {
    if(digitalRead(buttonUP) == 0) {
      current_value -= 1;
      if(current_value < 0) {
        current_value = 0;
      }
      else {
        last_results(current_value);
      }
      while(digitalRead(buttonUP) == 0) {
        digitalWrite(led, LOW);
      }
      digitalWrite(led, HIGH);
    }
    delay(10);
    if(digitalRead(buttonDOWN) == 0) {
      current_value += 1;
      if(current_value > 5) {
        current_value = 5;
      }
      else {
        last_results(current_value);
      }
      while(digitalRead(buttonDOWN) == 0) {
        digitalWrite(led, LOW);
      }
      digitalWrite(led, HIGH);
    }
    delay(10);
  }
  while(digitalRead(buttonOK) == 0) { delay(10); }



  // for(int i = 8; i < 32; i++) {
  //   int last = pow(2, i);
  //   Serial.print("\nFind primes until ");
  //   Serial.print(last);
  //   find_primes(last);
  //   delay(10);
  // }


  // select the range for the new calculation *****************
  u8g2.clearBuffer();
  u8g2.setCursor(0, 0);
  u8g2.setFont(u8g2_font_profont22_tn);
  u8g2.print("Primes to");
  current_value = 0;
  show_last(current_value);
  while(digitalRead(buttonOK) == 1) {
    if(digitalRead(buttonDOWN) == 0) {
      current_value -= 1;
      if(current_value < 0) {
        current_value = 0;
      }
      else {
        show_last(current_value);
      }
      while(digitalRead(buttonDOWN) == 0) {
        digitalWrite(led, LOW);
      }
      digitalWrite(led, HIGH);
    }
    delay(10);
    if(digitalRead(buttonUP) == 0) {
      current_value += 1;
      if(current_value > 10) {
        current_value = 10;
      }
      else {
        show_last(current_value);
      }
      while(digitalRead(buttonUP) == 0) {
        digitalWrite(led, LOW);
      }
      digitalWrite(led, HIGH);
    }
    delay(10);
  }


  // start calculating in micros for higher precision for the first 7 calculations
  long last = scope[current_value];
  found = 4;   // we already know 2, 3, 5, 7
  Serial.println("\n\nPrime v5.3");
  Serial.print("Primes until ");
  Serial.println(last);

  
  start = micros();      
  long largest_divider = (long)(sqrt(last)); 
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
  long dot = millis();
  int column = 0;
  for(long number = largest_divider + 2; number < last; number += 2)
  {
    found += is_prime_fast(number);
    if((millis() - dot) > 1000) {
      Serial.print(".");
      dot = millis();
      column += 1;
      if(column % 2 == 0) {
        digitalWrite(led, HIGH);
      }
      else
      {
        digitalWrite(led, LOW);
      }
      u8g2.setCursor(0, 32);
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.println(number);
      u8g2.print(number / (last / 100.0));
      u8g2.println("%   ");
      // timer = (micros() - start)/1000000.0;
      // display.print(timer);
      // display.println(" seconds ");
      // int hours = (int)timer/3600;
      // int minutes = (int)(timer/60 - hours*60);
      // display.print(hours);
      // display.print("h ");
      // display.print(minutes);
      // display.print("min ");
      u8g2.sendBuffer();
      if(column > 40) {
        column = 0;
        elapsed_time(dot/1000);
        Serial.print(" - ");
        Serial.print(number);
        Serial.print(" ");
        Serial.print((int)(number / (last / 100)));
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
  Serial.print(reference[current_value]);
  Serial.print(".\nThis took ");
  Serial.print(duration, 3);
  Serial.print(" seconds. ");
  elapsed_time(duration);
  EEPROM.put(current_value * 4 + 1, duration);
  boolean ok1 = EEPROM.commit();
  Serial.println((ok1) ? "- OK" : "Commit failed");
  Serial.print("\n");
}

void loop() {
  Serial.print(".");
  delay(5000);
}

