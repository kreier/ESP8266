#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

//U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14);
U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14);

void setup() {
  u8g2.begin();
}

void loop() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0,7,"Hello World!");
    u8g2.drawStr(0,15,"abcdefghijklmnopqrstuvwxyz");
    u8g2.drawStr(0,23,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    u8g2.drawStr(0,31,"01234568790123465789012345");
  } while ( u8g2.nextPage() );
  delay(1000);

}
