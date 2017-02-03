#ifndef _ILI9341_ESP32_H_
#define _ILI9341_ESP32_H_

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

#include "Arduino.h"

class ILI9341_ESP32 {
  public:
      ILI9341_ESP32(int8_t mosi, int8_t miso, int8_t clk, int8_t cs, int8_t dc, int8_t reset=0);
      void begin(uint32_t busFrequency);                                        //init driveer and set a bus frequency
      void begin();                                                             //init driver, sets 20MHz as default bus frequency
      void clearDisplay();
      void setRotation(uint8_t m);
      void invertDisplay(bool invert);                                          //inverts the screens content
      void fillScreen(uint16_t color);                                          //fills screen with a given color
      uint16_t height();                                                        //returns the height of this display depending on the current rotation
      uint16_t width();
      //pixels
      void drawPixel(uint16_t x, uint16_t y, uint16_t color);
      //lines
      void drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
    	void drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
      void drawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2,  uint16_t color);
      //figures
      void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
      void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
      void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
      void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
      void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
      void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
      void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color);
      void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color);

      //bitmaps
      void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
      void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
      void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
      void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
      void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
      void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);


  private:
      uint16_t _width;        //internal width, e.g. after rotating the screen
      uint16_t _height;       //internal height, e.g. after rotating the screen
      uint8_t pin_miso, pin_mosi, pin_clk, pin_cs, pin_dc, pin_reset;
      uint8_t rotation;
      uint8_t buf[64];
      uint32_t _busFrequency;                                                   //bus frequency, ESP32 can do up to 80MHz
      void _writeCommand(uint8_t command);
      void _writeData(uint8_t * data, size_t length);
      void _writeData(uint8_t data);
      void _setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
      void _drawPixels(size_t length);
      void _fillBuff(uint16_t color);
      void _fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
      void _drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);

};

#endif
