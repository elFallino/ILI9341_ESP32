#include "ILI9341_ESP32.h"
#include "ILI9341_ESP32_commands.h"
#include "SPI.h"
#include "Arduino.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif


/*
static inline void spi_begin(void) __attribute__((always_inline));
static inline void spi_begin(void) { SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0)); };             //NOTE: found some notes in google that most ILI9341 can handle that speed when only receiving data. Not checked if this speed is really applied.
static inline void spi_end(void) __attribute__((always_inline));
static inline void spi_end(void) {};
*/

#define SPI_BEGIN SPI.beginTransaction(SPISettings(_busFrequency, MSBFIRST, SPI_MODE0));
#define SPI_END

void ILI9341_ESP32::_writeCommand(uint8_t command) {
  digitalWrite(pin_dc, LOW);
  digitalWrite(pin_clk, LOW);
  SPI.write(command);
}


void ILI9341_ESP32::_writeData(uint8_t * data, size_t length){
  digitalWrite(pin_dc, HIGH);
  SPI.writeBytes(data, length);
};

void ILI9341_ESP32::_writeData(uint8_t data){
  digitalWrite(pin_dc, HIGH);
  SPI.write(data);
};


ILI9341_ESP32::ILI9341_ESP32(int8_t mosi, int8_t miso, int8_t clk, int8_t cs, int8_t dc, int8_t reset){
  pin_miso = miso;
  pin_mosi = mosi;
  pin_clk  = clk;
  pin_cs   = cs;
  pin_dc   = dc;
  pin_reset= reset;

  rotation = 0;
  _width   = TFT_WIDTH;
  _height  = TFT_HEIGHT;
  _busFrequency=20000000;
}


void ILI9341_ESP32::begin(){
  if (pin_reset > 0) {
    pinMode(pin_reset, OUTPUT);
    digitalWrite(pin_reset, LOW);
  }

  pinMode(pin_dc, OUTPUT);
  pinMode(pin_cs, OUTPUT);

  SPI.begin(pin_clk, pin_miso, pin_mosi, pin_cs);  //sck, miso, mosi, ss

  // toggle RST low to reset
  if (pin_reset > 0) {
    digitalWrite(pin_reset, HIGH);
    delay(5);
    digitalWrite(pin_reset, LOW);
    delay(20);
    digitalWrite(pin_reset, HIGH);
    delay(150);
  }


  SPI_BEGIN

  _writeCommand(0xEF);
  _writeData(0x03);
  _writeData(0x80);
  _writeData(0x02);

  _writeCommand(0xCF);                                                           //
  _writeData(0x00);
  _writeData(0XC1);
  _writeData(0X30);

  _writeCommand(0xED);                                                           //
  _writeData(0x64);
  _writeData(0x03);
  _writeData(0X12);
  _writeData(0X81);

  _writeCommand(0xE8);                                                           //
  _writeData(0x85);
  _writeData(0x00);
  _writeData(0x78);

  _writeCommand(0xCB);                                                           //
  _writeData(0x39);
  _writeData(0x2C);
  _writeData(0x00);
  _writeData(0x34);
  _writeData(0x02);

  _writeCommand(0xF7);                                                           //
  _writeData(0x20);

  _writeCommand(0xEA);                                                           //
  _writeData(0x00);
  _writeData(0x00);

  _writeCommand(ILI9341_PWCTR1);    //Power control
  _writeData(0x23);   //VRH[5:0]

  _writeCommand(ILI9341_PWCTR2);    //Power control
  _writeData(0x10);   //SAP[2:0];BT[3:0]

  _writeCommand(ILI9341_VMCTR1);    //VCM control
  _writeData(0x3e); //
  _writeData(0x28);

  _writeCommand(ILI9341_VMCTR2);    //VCM control2
  _writeData(0x86);  //--

  _writeCommand(ILI9341_MADCTL);    // Memory Access Control
  _writeData(0x58);//_writeData(0x48);                                            //Note: Set Bit 5 to enable line wrap when page end is reached. Used for very fast writing of Bitmaps

  _writeCommand(ILI9341_PIXFMT);     //Set pixel format
  _writeData(0x55);                  //0x55= 16 Bit Pixel

  _writeCommand(ILI9341_FRMCTR1);    //Frame Rate
  _writeData(0x00);
  _writeData(0x18);

  _writeCommand(ILI9341_DFUNCTR);    // Display Function Control
  _writeData(0x08);
  _writeData(0x82);
  _writeData(0x27);

  _writeCommand(0xF2);               // 3Gamma Function Disable
  _writeData(0x00);

  _writeCommand(ILI9341_GAMMASET);    //Gamma curve selected
  _writeData(0x01);

  _writeCommand(ILI9341_GMCTRP1);    //Set Gamma positive correction
  _writeData(0x0F);
  _writeData(0x31);
  _writeData(0x2B);
  _writeData(0x0C);
  _writeData(0x0E);
  _writeData(0x08);
  _writeData(0x4E);
  _writeData(0xF1);
  _writeData(0x37);
  _writeData(0x07);
  _writeData(0x10);
  _writeData(0x03);
  _writeData(0x0E);
  _writeData(0x09);
  _writeData(0x00);

  _writeCommand(ILI9341_GMCTRN1);    //Set Gamma negative correction
  _writeData(0x00);
  _writeData(0x0E);
  _writeData(0x14);
  _writeData(0x03);
  _writeData(0x11);
  _writeData(0x07);
  _writeData(0x31);
  _writeData(0xC1);
  _writeData(0x48);
  _writeData(0x08);
  _writeData(0x0F);
  _writeData(0x0C);
  _writeData(0x31);
  _writeData(0x36);
  _writeData(0x0F);

  _writeCommand(ILI9341_SLPOUT);    //Exit Sleep
  SPI_END
  delay(120);
  SPI_BEGIN
  _writeCommand(ILI9341_DISPON);    //Display on
  SPI_END
};


void ILI9341_ESP32::_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
  _writeCommand(ILI9341_CASET); // Column addr set
  _writeData((x0 >> 8));
  _writeData(x0 & 0xFF);
  _writeData((x1 >> 8));
  _writeData(x1);    //X0 = XSTART, X1 = XEND

  _writeCommand(ILI9341_PASET); // Row addr set
  _writeData((y0 >> 8));
  _writeData(y0 & 0xFF);
  _writeData((y1 >> 8));
  _writeData(y1);    //Y0 = YSTART, Y1 = YEND

  _writeCommand(ILI9341_RAMWR); // write to RAM
};


void ILI9341_ESP32::begin(uint32_t busFrequency){
  _busFrequency = busFrequency;
  begin();
};





void ILI9341_ESP32::drawPixel(uint16_t x, uint16_t y, uint16_t color){
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
  SPI_BEGIN
  _setAddrWindow(x,y,x+1,y+1);
  digitalWrite(pin_dc, HIGH);
  SPI.write16(color);
  SPI_END
};





void ILI9341_ESP32::_drawPixels(size_t length){
  digitalWrite(pin_dc, HIGH);
  while(length>=32){
    SPI.writeBytes(buf, 64);
    length-=32;
  };
  if(length>0)SPI.writeBytes(buf, length*2);       //write rest if any
};



void ILI9341_ESP32::fillScreen(uint16_t color){
  fillRect(0, 0, _width, _height, color);
}

void ILI9341_ESP32::clearDisplay (){
  fillRect(0, 0, _width, _height, ILI9341_BLACK);
}


/*******************************************************************************

---------- drawing lines ----------

*******************************************************************************/

/*
  draws a vertical line
*/
void ILI9341_ESP32::drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color){
  fillRect(x, y, 1, h, color);
};

/*
  draws a horizontal link
*/
void ILI9341_ESP32::drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color){
  fillRect(x, y, w, 1, color);
};

/*
  draws a line from any given point to another point on screen
*/
void ILI9341_ESP32::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,  uint16_t color){

    _fillBuff(color);
    uint16_t steep = abs(y2-y1) > abs(x2 - x1);

    if (steep) {
      _swap_int16_t(x1, y1);
      _swap_int16_t(x2, y2);
    }

    if (x1 > x2) {
      _swap_int16_t(x1, x2);
      _swap_int16_t(y1, y2);
    }

    int16_t dx, dy;
    dx = x2 - x1;
    dy = abs(y2 - y1);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y1 < y2) {
      ystep = 1;
    } else {
      ystep = -1;
    }

    uint16_t pxlen = 0;
    SPI_BEGIN
    for (; x1<=x2; x1++) {
      err -= dy;
      if (err < 0) {
        if(steep)
          _setAddrWindow(y1, x1-pxlen, y1, x1-1);
        else
          _setAddrWindow(x1-pxlen, y1, x1-1, y1);
        _drawPixels(pxlen);
        y1 += ystep;
        err += dx;
        pxlen=1;
      } else {
        pxlen++;
      }
    }
    SPI_END

    //displaying the rest
    if(steep)
      _setAddrWindow(y1, x1-pxlen, y1, x1-1);
    else
      _setAddrWindow(x1-pxlen, y1, x1-1, y1);
    _drawPixels(pxlen);
  }


/*******************************************************************************

---------- drawing figures ----------

*******************************************************************************/

/*
  draws a rect (outlines only)
*/
void ILI9341_ESP32::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
    _fillBuff(color);
    SPI_BEGIN

    //draw top line
    _setAddrWindow(x,y,x+w,y);
    _drawPixels(w);

    //draw bottom line
    _setAddrWindow(x,y+h+1,x+w,y+h-1);
    _drawPixels(w);

    //draw right line
    _setAddrWindow(x+w-1,y+1,x+w-1,y+h-2);
    _drawPixels(h);

    //draw left line
    _setAddrWindow(x,y+1,x,y+h-2);
    _drawPixels(h);

    SPI_END
  }


/*
  draws a filled rect
*/
void ILI9341_ESP32::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
  _fillBuff(color);
  SPI_BEGIN
  _setAddrWindow(x,y,x+w-1,y+h-1);
  _drawPixels(w*h);
  SPI_END
}

/*
  draws a circle (outline only)
  this function is a modified copy taken from Adafruit_GFX
*/
void ILI9341_ESP32::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  _fillBuff(color);
  SPI_BEGIN
  _setAddrWindow(x0, y0+r, x0, y0+r);
  _drawPixels(1);
  _setAddrWindow(x0, y0-r, x0, y0-r);
  _drawPixels(1);
  _setAddrWindow(x0+r, y0, x0+r, y0);
  _drawPixels(1);
  _setAddrWindow(x0-r, y0, x0-r, y0);
  _drawPixels(1);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    _setAddrWindow(x0 + x, y0 + y, x0 + x, y0 + y);
    _drawPixels(1);
    _setAddrWindow(x0 - x, y0 + y, x0 - x, y0 + y);
    _drawPixels(1);
    _setAddrWindow(x0 + x, y0 - y, x0 + x, y0 - y);
    _drawPixels(1);
    _setAddrWindow(x0 - x, y0 - y, x0 - x, y0 - y);
    _drawPixels(1);
    _setAddrWindow(x0 + y, y0 + x, x0 + y, y0 + x);
    _drawPixels(1);
    _setAddrWindow(x0 - y, y0 + x, x0 - y, y0 + x);
    _drawPixels(1);
    _setAddrWindow(x0 + y, y0 - x, x0 + y, y0 - x);
    _drawPixels(1);
    _setAddrWindow(x0 - y, y0 - x, x0 - y, y0 - x);
    _drawPixels(1);
  } //while
  SPI_END
}


void ILI9341_ESP32::_drawCircleHelper( int16_t x0, int16_t y0,
 int16_t r, uint8_t cornername, uint16_t color) {
  //_fillbuff(color);     //maybe we could skip that since previous method that uses that one already filled buffer

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  SPI_BEGIN
  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      _setAddrWindow(x0 + x, y0 + y, x0 + x, y0 + y);
      _drawPixels(1);
      _setAddrWindow(x0 + y, y0 + x, x0 + y, y0 + x);
      _drawPixels(1);
    }
    if (cornername & 0x2) {
      _setAddrWindow(x0 + x, y0 - y, x0 + x, y0 - y);
      _drawPixels(1);
      _setAddrWindow(x0 + y, y0 - x, x0 + y, y0 - x);
      _drawPixels(1);
    }
    if (cornername & 0x8) {
      _setAddrWindow(x0 - y, y0 + x, x0 - y, y0 + x);
      _drawPixels(1);
      _setAddrWindow(x0 - x, y0 + y, x0 - x, y0 + y);
      _drawPixels(1);
    }
    if (cornername & 0x1) {
      _setAddrWindow(x0 - y, y0 - x, x0 - y, y0 - x);
      _drawPixels(1);
      _setAddrWindow(x0 - x, y0 - y, x0 - x, y0 - y);
      _drawPixels(1);
    }
  }
  SPI_END
}


/*
  draws a filled circle
*/
void ILI9341_ESP32::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
  fillRect(x0, y0-r, 1 ,2*r+1, color);
  _fillCircleHelper(x0, y0, r, 3, 0, color);
}

/*
  helper function to draw a circle, can also be used to help drawing round rects
  this function is a modified copy taken from Adafruit_GFX
*/
void ILI9341_ESP32::_fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
 uint8_t cornername, int16_t delta, uint16_t color) {
  //_fillbuff(color);     //maybe we could skip that since previous method that uses that one already filled buffer
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  SPI_BEGIN
  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {       //right half of a circle
      _setAddrWindow(x0+x, y0-y, x0+x,  (y0-y) + (2*y+1+delta));
      _drawPixels(2*y+1+delta);
      _setAddrWindow(x0+y, y0-x, x0+y, (y0-x) + (2*x+1+delta));
      _drawPixels(2*y+1+delta);
    }
    if (cornername & 0x2) {       //left half of a circle
      _setAddrWindow(x0-x, y0-y, x0-x,  (y0-y) + (2*y+1+delta));
      _drawPixels(2*y+1+delta);
      _setAddrWindow(x0-y, y0-x, x0-y, (y0-x) + (2*x+1+delta));
      _drawPixels(2*y+1+delta);
    }
  }
  SPI_END
}


void ILI9341_ESP32::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
};

/*
  draws a filled triangle
  this function is a modified copy taken from Adafruit_GFX
*/
void ILI9341_ESP32::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
      _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
      _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
      _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
      a = b = x0;
      if(x1 < a)      a = x1;
      else if(x1 > b) b = x1;
      if(x2 < a)      a = x2;
      else if(x2 > b) b = x2;
      fillRect(a, y0, b-a+1, 1, color);
      return;
    }

    int16_t
      dx01 = x1 - x0,
      dy01 = y1 - y0,
      dx02 = x2 - x0,
      dy02 = y2 - y0,
      dx12 = x2 - x1,
      dy12 = y2 - y1;
    int32_t
      sa   = 0,
      sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
      a   = x0 + sa / dy01;
      b   = x0 + sb / dy02;
      sa += dx01;
      sb += dx02;
      /* longhand:
      a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
      b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
      */
      if(a > b) _swap_int16_t(a,b);
      fillRect(a, y, b-a+1, 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
      a   = x1 + sa / dy12;
      b   = x0 + sb / dy02;
      sa += dx12;
      sb += dx02;
      /* longhand:
      a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
      b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
      */
      if(a > b) _swap_int16_t(a,b);
      fillRect(a, y, b-a+1, 1, color);
    }
};


/*
  draw a rounded rectangle (outline only)
  this function is a modified copy taken from Adafruit_GFX
*/
void ILI9341_ESP32::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color){
  // smarter version
  drawFastHLine(x+radius  , y         , w-2*radius, color); // Top
  drawFastHLine(x+radius  , y+h-1     , w-2*radius, color); // Bottom
  drawFastVLine(x         , y+radius  , h-2*radius, color); // Left
  drawFastVLine(x+w-1     , y+radius  , h-2*radius, color); // Right
  // draw four corners
  _drawCircleHelper(x+radius    , y+radius    , radius, 1, color);
  _drawCircleHelper(x+w-radius-1, y+radius    , radius, 2, color);
  _drawCircleHelper(x+w-radius-1, y+h-radius-1, radius, 4, color);
  _drawCircleHelper(x+radius    , y+h-radius-1, radius, 8, color);
};


/*
  draw a filled rounded rectangle
  this function is a modified copy taken from Adafruit_GFX
*/
void ILI9341_ESP32::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color){
  // smarter version
  fillRect(x+radius, y, w-2*radius, h, color);
  // draw four corners
  _fillCircleHelper(x+w-radius-1, y+radius, radius, 1, h-2*radius-1, color);
  _fillCircleHelper(x+radius    , y+radius, radius, 2, h-2*radius-1, color);
};



/*******************************************************************************

---------- drawing Bitmaps ----------

*******************************************************************************/

/*
  drawBitmap w/o background (=transparent) for ROM resident (PROGMEM) bitmaps
*/
void ILI9341_ESP32::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;
  uint32_t xpos = x;
  uint32_t ypos = y;
  uint32_t pxlen = 0;    //count of drawable pixels since last SPI transfer

  _fillBuff(color);

  SPI_BEGIN
  for(j=0; j<(w*h); j++) {
    if(j & 7) byte <<= 1;
    else      byte   = pgm_read_byte(bitmap + (j / 8));

    if(byte & 0x80) {
      if(pxlen==0)                                                              //change from low to high
        _setAddrWindow(xpos, ypos, x+w-1, ypos);                                 //set new adress window
      pxlen++;
    } else {
      if(pxlen>0) {                                                             //change from high to low
        _drawPixels(pxlen);                                                      //draw pixels
        pxlen=0;
      }
    }
    xpos++;

    if(xpos==x+w) {                                                             //already reached end of a line
      _drawPixels(pxlen);                                                        //draw pixels
      pxlen=0;
      xpos=x;
      ypos++;
    }
  }
  SPI_END
};

/*
  drawBitmap with background for ROM resident (PROGMEM) bitmaps
*/
void ILI9341_ESP32::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg){
  fillRect(x,y,w,h,bg);
  drawBitmap(x,y,bitmap,w,h,color);
};


/*
  drawBitmap w/o background (=transparent) for RAM resident (not PROGMEM) bitmaps
*/
void ILI9341_ESP32::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;
  uint32_t xpos = x;
  uint32_t ypos = y;
  uint32_t pxlen = 0;    //count of drawable pixels since last SPI transfer

  _fillBuff(color);

  SPI_BEGIN
  for(j=0; j<(w*h); j++) {
    if(j & 7) byte <<= 1;
    else      byte   = bitmap[j*byteWidth + i/8];

    if(byte & 0x80) {
      if(pxlen==0)                                                              //change from low to high
        _setAddrWindow(xpos, ypos, x+w-1, ypos);                                 //set new adress window
      pxlen++;
    } else {
      if(pxlen>0) {                                                             //change from high to low
        _drawPixels(pxlen);                                                      //draw pixels
        pxlen=0;
      }
    }
    xpos++;

    if(xpos==x+w) {                                                             //already reached end of a line
      _drawPixels(pxlen);                                                        //draw pixels
      pxlen=0;
      xpos=x;
      ypos++;
    }
  }
  SPI_END
};

/*
  drawBitmap with background for RAM resident (not PROGMEM) bitmaps
*/
void ILI9341_ESP32::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg){
  fillRect(x,y,w,h,bg);
  drawBitmap(x,y,bitmap,w,h,color);
};


/*
Draws XBitMap files (*.xbm), exported from GIMP,
HowTo: Export image from GIMP to *.xbm, rename that file to *.c and open in a texteditor.
This array can be displayed using this function


THIS FUNCTION NEEDS TESTING
*/
void ILI9341_ESP32::drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){
  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;
  uint32_t xpos = x;
  uint32_t ypos = y;
  uint32_t pxlen = 0;    //count of drawable pixels since last SPI transfer

  _fillBuff(color);

  SPI_BEGIN
  for(j=0; j<(w*h); j++) {
    for(i=0; i<w; i++ ) {
      if(i & 7) byte >>= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);

      if(byte & 0x01) {
        if(pxlen==0)                                                              //change from low to high
          _setAddrWindow(xpos, ypos, x+w-1, ypos);                                 //set new adress window
          pxlen++;
        } else {
          if(pxlen>0) {                                                             //change from high to low
            _drawPixels(pxlen);                                                      //draw pixels
            pxlen=0;
          }
        }
        xpos++;

        if(xpos==x+w) {                                                             //already reached end of a line
          _drawPixels(pxlen);                                                        //draw pixels
          pxlen=0;
          xpos=x;
          ypos++;
        }
      } //for i
    } //for j
  SPI_END
}


void ILI9341_ESP32::drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg){
  fillRect(x,y,w,h,bg);
  drawXBitmap(x,y,bitmap,w,h,color);
};


/*******************************************************************************

---------- other public functions ----------

*******************************************************************************/


/*
  set rotation of screen in 90deg steps (0=0deg, 1=90deg, 2 = 180deg, 3 = 270deg)
*/
void ILI9341_ESP32::setRotation(uint8_t m) {
  SPI_BEGIN
  _writeCommand(ILI9341_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     _writeData(MADCTL_MX | MADCTL_BGR);
     _width  = TFT_WIDTH;
     _height = TFT_HEIGHT;
     break;
   case 1:
     _writeData(MADCTL_MV | MADCTL_BGR);
     _width  = TFT_HEIGHT;
     _height = TFT_WIDTH;
     break;
  case 2:
    _writeData(MADCTL_MY | MADCTL_BGR);
     _width  = TFT_WIDTH;
     _height = TFT_HEIGHT;
    break;
   case 3:
     _writeData(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     _width  = TFT_HEIGHT;
     _height = TFT_WIDTH;
     break;
  }
  SPI_END
}


/*
  invert whole display
*/
void ILI9341_ESP32::invertDisplay(bool invert) {
  SPI_BEGIN
  _writeCommand(invert ? ILI9341_INVON : ILI9341_INVOFF);
  SPI_END
}


/*
  returns the width of the display dpending on the current rotation
*/
uint16_t ILI9341_ESP32::width(){ return _width; };


/*
  returns the height of the display dpending on the current rotation
*/
uint16_t ILI9341_ESP32::height() { return _height; };


/*******************************************************************************

---------- private functions ----------

*******************************************************************************/


/*
  fills the internal buffer with a given color
*/
void ILI9341_ESP32::_fillBuff(uint16_t color) {
  for(uint8_t i=0; i<64; i+=2){                                                 //Fill buffer. We don't take care yet if we do not need whe whole buffer
      buf[i]=color>>8;                                                          //...maybe we should take care later
      buf[i+1]=color;
  }
}
