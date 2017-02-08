#ifndef _ILI9341_ESP32_H_
#define _ILI9341_ESP32_H_

#ifndef ESP32
  error("Hardware seems not to be an ESP32 - driver might not work")
#endif


#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0D
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

//definiton of colors

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
#include <pgmspace.h>
#include "gfxfont.h"
#include "glcdfont.c"	//default fonts

class ILI9341_ESP32 : public Print {
  public:
      ILI9341_ESP32(int8_t mosi, int8_t miso, int8_t clk, int8_t cs, int8_t dc, int8_t reset=0);
      void begin(uint32_t busFrequency);                                        //init driveer and set a bus frequency
      void begin();                                                             //init driver, sets 20MHz as default bus frequency
      void clearDisplay();
      void setRotation(uint8_t m);
      void invertDisplay(bool invert);                                          //inverts the screens content
      void fillScreen(uint16_t color);                                          //fills screen with a given color
      uint8_t readcommand8(uint8_t reg, uint8_t index = 0);
      uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
      uint16_t height();                                                        //returns the height of this display depending on the current rotation
      uint16_t width();
      uint8_t getRotation(void) const;
      int16_t getCursorX(void) const;
      int16_t getCursorY(void) const;
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
      //text and fonts
      void setCursor(int16_t x, int16_t y);
    	void setTextColor(uint16_t color);
      void setTextColor(uint16_t color, uint16_t bgcolor);
    	void setTextSize(uint8_t s);
      void setTextWrap(bool wrap);
      void cp437(boolean x=true);
    	void setFont(const GFXfont *f = NULL);
      void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
      void getTextBounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
      void getTextBounds(const __FlashStringHelper *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
      size_t write(uint8_t);

  private:
      uint16_t _width;        //internal width, e.g. after rotating the screen
      uint16_t _height;       //internal height, e.g. after rotating the screen
      uint8_t pin_miso, pin_mosi, pin_clk, pin_cs, pin_dc, pin_reset;
      uint8_t _rotation;
      uint8_t buf[64];
      uint32_t _busFrequency;                                                   //bus frequency, ESP32 can do up to 80MHz

      int16_t cursor_x, cursor_y;
      int16_t textcolor, textbgcolor;
      uint8_t  textsize;
      boolean _wrap;   // If set, 'wrap' text at right edge of display
      boolean _cp437; // If set, use correct CP437 charset (default is off)
      GFXfont   *gfxFont;

      void _writeCommand(uint8_t command);
      void _writeData(uint8_t * data, size_t length);
      void _writeData(uint8_t data);
      void _setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
      void _drawPixels(size_t length);
      void _fillBuff(uint16_t color);
      void _fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
      void _drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
      void _drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bgcolor, uint8_t size);

};

#endif
