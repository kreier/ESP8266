// DS3231 get RTC on ESP8266 with 128x64 OLED SPI display
// buffer 16x8 for 8x8 font 
// 2020/07/13 v0.2

#include <Arduino.h>
#include <U8x8lib.h>
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>

U8X8_SH1106_128X64_NONAME_4W_HW_SPI u8x8(/* cs=*/ D8, /* dc=*/ D3, /* reset=*/ D4);

#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8X8LOG u8x8log;

DS3231 rtc;
bool century = false;
bool h12Flag;
bool pmFlag;

String datetime;

void setup(void) {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nRTC clock");

  u8x8.begin();
  u8x8.setFont(u8x8_font_artosserif8_r);
  
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(1);		// 0: Update screen with newline, 1: Update screen for every char
  u8x8log.print("\nRTC clock\n");
}

void loop(void) {
  update_datetime();
  Serial.println(datetime);
  u8x8log.print(datetime);
  u8x8log.print("\n");
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
  //datetime += ":";
  //if(rtc.getSecond() < 10) datetime += "0";
  //datetime += rtc.getSecond(); 
}
