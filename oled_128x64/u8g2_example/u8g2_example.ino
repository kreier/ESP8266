// Drive displays with the U8g2 library

#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <time.h>
#include <math.h>
#include <ESP_EEPROM.h>

U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 12, 14);   // wide Lolin ESP8266 board
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 4, 5);    // integrated board
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 4, 5);    // narrow board

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0,20,"Hello World!");
  u8g2.sendBuffer();
}
