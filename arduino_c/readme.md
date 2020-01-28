# Arduino C for ESP8266 robot

Just some standard definitions for our T400 to have no confusion:

- SDA is pin 14 (D5)
- SCL is pin 12 (D6)

For the displays we use the U8x8lib.h and U8g2lib.h with the respective contructors. Mostly we have to adjust the data SDA and the clock SCL Pins:

- U8X8_SH1106_128X64_NONAME_SW_I2C  u8x8( [SCL], [SDA] );
- U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8( [SCL], [SDA], [reset] );

- U8G2_SH1106_128X64_NONAME_F_SW_I2C  u8g2( [orientation], U8G2_R0,  [SCL], [SDA] );
- U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2( [orientation], U8G2_R0,  [SCL], [SDA], [reset] );


## Graphics library u8g2

Since we are only using the u8g2 library we have only two options:

- U8G2_SH1106_128X64_NONAME_F_SW_I2C  u8g2(U8G2_R0, 12, 14);
- U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 12, 14, U8X8_PIN_NONE);


