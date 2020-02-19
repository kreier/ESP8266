#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14);
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
}

void loop() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 7,"Lorem ipsum dolor sit est,");
    u8g2.drawStr(0,14,"consectetur elit, sed do");
    u8g2.drawStr(0,21,"ut labore et dolore magna");
    u8g2.drawStr(0,28,"aliqua. Et malesuada fames");
    u8g2.drawStr(0,35,"ac turpis egestas integer");
    u8g2.drawStr(0,42,"eget aliquet nibh. Ipsum");
    u8g2.drawStr(0,49,"suspendisse ultrices gravi");
    u8g2.drawStr(0,56,"da dictum fusce. Ac felis");
    u8g2.drawStr(0,63,"malesuada diam lacus eget.");
  } while ( u8g2.nextPage() );
  delay(1000);
}

/* here for ssd1306 in two color

void loop() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 7,"Lorem ipsum dolor sit est,");
    u8g2.drawStr(0,14,"consectetur elit, sed do");
    u8g2.drawStr(0,22,"ut labore et dolore magna");
    u8g2.drawStr(0,29,"aliqua. Et malesuada fames");
    u8g2.drawStr(0,37,"ac turpis egestas integer");
    u8g2.drawStr(0,43,"eget aliquet nibh. Ipsum");
    u8g2.drawStr(0,50,"suspendisse ultrices gravi");
    u8g2.drawStr(0,57,"da dictum fusce. Ac felis");
    u8g2.drawStr(0,64,"malesuada diam lacus eget.");
  } while ( u8g2.nextPage() );
  delay(1000);
}

 * sh1106 monochrome

void loop() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 7,"Lorem ipsum dolor sit est,");
    u8g2.drawStr(0,14,"consectetur elit, sed do");
    u8g2.drawStr(0,21,"ut labore et dolore magna");
    u8g2.drawStr(0,28,"aliqua. Et malesuada fames");
    u8g2.drawStr(0,35,"ac turpis egestas integer");
    u8g2.drawStr(0,42,"eget aliquet nibh. Ipsum");
    u8g2.drawStr(0,49,"suspendisse ultrices gravi");
    u8g2.drawStr(0,56,"da dictum fusce. Ac felis");
    u8g2.drawStr(0,63,"malesuada diam lacus eget.");
  } while ( u8g2.nextPage() );
  delay(1000);
}

 * and just 8 lines of text - easier to read!

void loop() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 8,"Lorem ipsum dolor sit est,");
    u8g2.drawStr(0,16,"consectetur elit, sed do");
    u8g2.drawStr(0,24,"ut labore et dolore magna");
    u8g2.drawStr(0,32,"aliqua. Et malesuada fames");
    u8g2.drawStr(0,40,"ac turpis egestas integer");
    u8g2.drawStr(0,48,"eget aliquet nibh. Ipsum");
    u8g2.drawStr(0,56,"suspendisse ultrices gravi");
    u8g2.drawStr(0,64,"da dictum fusce. Ac felis");
  } while ( u8g2.nextPage() );
  delay(1000);
}
*/
