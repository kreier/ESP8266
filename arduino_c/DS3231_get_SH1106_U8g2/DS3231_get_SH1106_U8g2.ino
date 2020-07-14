// DS3231 get RTC on ESP8266 with 128x64 OLED SPI display 
// buffer 26x9 for 5x7 font
// 2020/07/14 v0.3

#include <Arduino.h>
#include <U8x8lib.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>

U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D8, /* dc=*/ D3, /* reset=*/ D4);

#define U8LOG_WIDTH 26
#define U8LOG_HEIGHT 9
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8G2LOG u8g2log;

DS3231 rtc;
bool century = false;
bool h12Flag;
bool pmFlag;

String datetime;

void setup(void) {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nRTC clock");

  u8g2.begin();
  u8g2.setFont(u8g2_font_5x7_mf);
  
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setRedrawMode(0);		// 0: Update screen with newline, 1: Update screen for every char
  u8g2log.print("\nRTC clock\n");
}

void loop(void) {
  update_datetime();
  Serial.println(datetime);
  u8g2log.print(datetime);
  u8g2log.print("\n");
  delay(10000);
}

void update_datetime() {
  datetime = "20";
  datetime += rtc.getYear();
  datetime += "-";
  if(rtc.getMonth(century) < 10) datetime += "0";
  datetime += rtc.getMonth(century);
  datetime += "-";
  if(rtc.getDate() < 10) datetime += "0";
  datetime += rtc.getDate();
  datetime += " ";
  if(rtc.getHour(h12Flag, pmFlag) < 10) datetime += "0";  
  datetime += rtc.getHour(h12Flag, pmFlag); //24-hr
  datetime += ":";
  if(rtc.getMinute() < 10) datetime += "0";
  datetime += rtc.getMinute();
  datetime += ":";
  if(rtc.getSecond() < 10) datetime += "0";
  datetime += rtc.getSecond();
  datetime += " ";
  datetime += rtc.getTemperature();
}
