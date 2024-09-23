/**************************************************************************
 This is based on the Monochrome OLEDs example, for SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#ifndef I2C_OLED_H_INCLUDED
#define I2C_OLED_H_INCLUDED


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class I2C_OLED {

private:
  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  // The pins for I2C are defined by the Wire-library.
  // On an arduino UNO:       A4(SDA), A5(SCL)
  // On an arduino MEGA 2560: 20(SDA), 21(SCL)
  // On an arduino LEONARDO:   2(SDA),  3(SCL), ...

	  uint8_t OLED_RESET     =4; // Reset pin # (or -1 if sharing Arduino reset pin)
  uint8_t SCREEN_WIDTH =128 ;// OLED display width, in pixels
  uint8_t SCREEN_HEIGHT =64 ;// OLED display height, in pixels
  uint8_t SCREEN_ADDRESS= 0x3C;///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
  uint8_t NUMFLAKES    = 10 ;// Number of snowflakes in the animation example
  uint8_t LOGO_HEIGHT  = 16;
  uint8_t LOGO_WIDTH   = 16;
  uint8_t XPOS  = 0 ;// Indexes into the 'icons' array in function below
  uint8_t YPOS   =1;
  uint8_t DELTAY =2;
  bool working;
/* 
  unsigned char PROGMEM logo_bmp[]= {
       0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000}
  ; */
  

public:
	I2C_OLED();
  //  void reset(void);

    void set(uint8_t screen_addr, uint8_t screen_width, uint8_t screen_height);
    void reset(void);
        void cls(void);
    void demo(String demo_name, bool cls);
    void keyin(bool cls, String title, String text);
    void power(bool on);
};


#endif // I2C_OLED_H_INCLUDED
