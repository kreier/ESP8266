#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

//U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
}

void loop() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 7,"Lorem ipsum dolor sit est,");
    u8g2.drawStr(0,15,"consectetur elit, sed do");
    u8g2.drawStr(0,23,"ut labore et dolore magna");
    u8g2.drawStr(0,31,"aliqua. Et malesuada fames");
    u8g2.drawStr(0,39,"ac turpis egestas integer");
    u8g2.drawStr(0,47,"eget aliquet nibh. Ipsum");
    u8g2.drawStr(0,55,"suspendisse ultrices gravi");
    u8g2.drawStr(0,64,"da dictum fusce. Ac felis");
  } while ( u8g2.nextPage() );
  delay(1000);

}
