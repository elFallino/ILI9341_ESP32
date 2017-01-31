#ifndef _ILI9341_ESP32_H_
#define _ILI9341_ESP32_H_

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_WHITE       0xffff      /* 255, 255, 255 */

#include "Arduino.h"

class ILI9341_ESP32 {
  public:
      ILI9341_ESP32(int8_t mosi, int8_t miso, int8_t clk, int8_t cs, int8_t dc, int8_t reset=0);
      void begin(uint32_t busFrequency);                                        //set a bus frequency
      void begin();                                                             //sets 1MHz as default bus frequency
      void clearDisplay();                                                      //set rotation of the screen in 90° steps (0=0°, 1=90°, ...)
      void setRotation(uint8_t m);
      void drawPixel(uint16_t x, uint16_t y, uint16_t color);
      void drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
    	void drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
      void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,  uint16_t color);

      void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
      void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

  private:
      uint16_t _width;                                                          //internal width, e.g. after rotating the screen
      uint16_t _height;                                                         //internal height, e.g. after rotating the screen
      uint8_t pin_miso, pin_mosi, pin_clk, pin_cs, pin_dc, pin_reset;           //pins used
      uint8_t rotation;                                                         //rotation of the screen in 90° steps (0=0°, 1=90°, ...)
      uint8_t buf[64];                                                          //internal buffer to hold up to 64 bytes
      uint32_t _busFrequency;                                                   //bus frequency, ESP32 can do up to 80MHz
      void writeCommand(uint8_t command);
      void writeData(uint8_t * data, size_t length);
      void writeData(uint8_t data);

      void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
      void drawPixels(size_t length);

};

#endif
