# Arduino C for ESP8266 robot

Just some standard definitions for our T400 to have no confusion:

- SDA is pin 14 (D5)
- SCL is pin 12 (D6)

For the displays we use the U8x8lib.h and U8g2lib.h with the respective contructors:

- U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 12, /* data=*/ 14);
- U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 12, /* data=*/ 14, /* reset=*/ U8X8_PIN_NONE);

- U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14);
- U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14, /* reset=*/ U8X8_PIN_NONE);

