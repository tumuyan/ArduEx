//extern "C" {}
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "i2c_oled.h"



I2C_OLED::I2C_OLED() {
}


void I2C_OLED::set(uint8_t screen_addr, uint8_t screen_width, uint8_t screen_height) {
  SCREEN_ADDRESS = screen_addr;
  SCREEN_WIDTH = screen_width;
  SCREEN_HEIGHT = screen_height;
}
/* 
void I2C_OLED::reset(void){
  uint8_t SCREEN_WIDTH =128 ;// OLED display width, in pixels
  uint8_t SCREEN_HEIGHT =64 ;// OLED display height, in pixels
  uint8_t SCREEN_ADDRESS= 0x3C;///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
} */

void I2C_OLED::cls(void) {
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    // Clear the buffer
    display.clearDisplay();
    display.display();
  }
}


void I2C_OLED::keyin(bool cls, String title, String text) {
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    if (cls) {
      display.clearDisplay();
      //  display.display();
    }

    {
      // display.ssd1306_command(0xAE);
      // Title background
      int i = 8;
      display.setTextSize(2);  // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);
      // text
      display.setTextColor(SSD1306_WHITE);
      display.print(text);

      display.fillRoundRect(0, 6 * i, display.width(), 2 * i, i, SSD1306_INVERSE);
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Draw 'inverse' text

      display.setCursor(5, 6 * i);  // Start at left-top corner
      display.println(title);
      display.display();
      // display.ssd1306_command(0xAF);
    }
  }
}



void I2C_OLED::power(bool on) {
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    if (on) {
      display.ssd1306_command(0xAF);
    } else {
      display.ssd1306_command(0xAe);
    }
  }
}

void I2C_OLED::demo(String demo_name, bool cls) {
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    return;
  }
  if (cls) {
    display.clearDisplay();
    display.display();
  }

  int i = 24;


  {
    if (demo_name == "testdrawline" || demo_name == "1") {
      //   testdrawline();
      display.drawLine(0, 0, display.width() - 1, display.height() - 1, SSD1306_WHITE);
      display.display();  // Update screen with each newly-drawn line

    }

    else if (demo_name == "testdrawrect" || demo_name == "2") {
      //   testdrawrect();

      display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, SSD1306_WHITE);
      display.display();  // Update screen with each newly-drawn rectangle

    } else if (demo_name == "testfillcircle" || demo_name == "3") {  //    testfillcircle();
      // The INVERSE color is used so circles alternate white/black
      display.fillCircle(display.width() / 2, display.height() / 2, 36, SSD1306_INVERSE);
      display.display();  // Update screen with each newly-drawn circle
    } else if (demo_name == "testdrawroundrect" || demo_name == "4") {

      //    testdrawroundrect();
      display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                            display.height() / 4, SSD1306_WHITE);
      display.display();
    }

    else if (demo_name == "testfillroundrect" || demo_name == "5") {

      display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                            display.height() / 4, SSD1306_INVERSE);
      display.display();
    }

    else if (demo_name == "testdrawtriangle" || demo_name == "6") {

      display.drawTriangle(
        display.width() / 2, display.height() / 2 - i,
        display.width() / 2 - i, display.height() / 2 + i,
        display.width() / 2 + i, display.height() / 2 + i, SSD1306_WHITE);
      display.display();
    } else if (demo_name == "testfilltriangle" || demo_name == "7") {
      display.fillTriangle(
        display.width() / 2, display.height() / 2 - i,
        display.width() / 2 - i, display.height() / 2 + i,
        display.width() / 2 + i, display.height() / 2 + i, SSD1306_INVERSE);
      display.display();
    } else if (demo_name == "testdrawchar" || demo_name == "8") {
      display.setTextSize(1);               // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);  // Draw white text
      display.setCursor(0, 0);              // Start at top-left corner
      display.cp437(true);                  // Use full 256 char 'Code Page 437' font

      // Not all the characters will fit on the display. This is normal.
      // Library will draw what it can and the rest will be clipped.
      for (int16_t i = 0; i < 256; i++) {
        if (i == '\n') display.write(' ');
        else display.write(i);
      }

      display.display();
    } else if (demo_name == "testdrawstyles" || demo_name == "9") {
      display.setTextSize(1);               // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);  // Draw white text
      display.setCursor(0, 0);              // Start at top-left corner
      display.println(F("Hello, world!"));

      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Draw 'inverse' text
      display.println(3.141592);

      display.setTextSize(2);  // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.print(F("0x"));
      display.println(0xDEADBEEF, HEX);

      display.display();
    } else if (demo_name == "testscrolltext" || demo_name == "10") {
      display.setTextSize(2);  // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 0);
      display.println(F("scroll"));
      display.display();  // Show initial text
      delay(100);

      // Scroll in various directions, pausing in-between:
      display.startscrollright(0x00, 0x0F);
      delay(2000);
      display.stopscroll();
      delay(1000);
      display.startscrollleft(0x00, 0x0F);
      delay(2000);
      display.stopscroll();
      delay(1000);
      display.startscrolldiagright(0x00, 0x07);
      delay(2000);
      display.startscrolldiagleft(0x00, 0x07);
      delay(2000);
      display.stopscroll();
    } else if (demo_name == "testdrawbitmap" || demo_name == "11") {
      static unsigned char PROGMEM logo_bmp[] = {
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
        0b00000000, 0b00110000
      };
      display.drawBitmap(
        (display.width() - LOGO_WIDTH) / 2,
        (display.height() - LOGO_HEIGHT) / 2,
        logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
    } else if (demo_name == "testanimate" || demo_name == "12") {
      /* 

 static unsigned char PROGMEM logo_bmp[]= {
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
  ;
  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
 */
    }

    else {
      for (uint8_t i = 0; i < 64; i++) {
        display.drawPixel(i, i, SSD1306_WHITE);
        display.display();
        delay(50);
      }
    }
  }

  // Draw a single pixel in white



  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
}
