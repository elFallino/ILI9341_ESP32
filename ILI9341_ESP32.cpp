#include "ILI9341_ESP32.h"
#include "ILI9341_ESP32_commands.h"
#include "SPI.h"
#include "Arduino.h"


static inline void spi_begin(void) __attribute__((always_inline));
static inline void spi_begin(void) { SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0)); };             //NOTE: found some notes in google that most ILI9341 can handle that speed when only receiving data. Not checked if this speed is really applied.
static inline void spi_end(void) __attribute__((always_inline));
static inline void spi_end(void) {};


void ILI9341_ESP32::writeCommand(uint8_t command) {
  digitalWrite(pin_dc, LOW);
  digitalWrite(pin_clk, LOW);
//  digitalWrite(pin_cs, LOW);
  SPI.write(command);
//  digitalWrite(pin_cs, HIGH);
}


void ILI9341_ESP32::writeData(uint8_t * data, size_t length){
  digitalWrite(pin_dc, HIGH);
  //digitalWrite(pin_cs, LOW);
  SPI.writeBytes(data, length);
  //digitalWrite(pin_cs, HIGH);
};

void ILI9341_ESP32::writeData(uint8_t data){
  digitalWrite(pin_dc, HIGH);
  //digitalWrite(pin_cs, LOW);
  SPI.write(data);
  //digitalWrite(pin_cs, HIGH);
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


  spi_begin();

  writeCommand(0xEF);
  writeData(0x03);
  writeData(0x80);
  writeData(0x02);

  writeCommand(0xCF);                                                           //
  writeData(0x00);
  writeData(0XC1);
  writeData(0X30);

  writeCommand(0xED);                                                           //
  writeData(0x64);
  writeData(0x03);
  writeData(0X12);
  writeData(0X81);

  writeCommand(0xE8);                                                           //
  writeData(0x85);
  writeData(0x00);
  writeData(0x78);

  writeCommand(0xCB);                                                           //
  writeData(0x39);
  writeData(0x2C);
  writeData(0x00);
  writeData(0x34);
  writeData(0x02);

  writeCommand(0xF7);                                                           //
  writeData(0x20);

  writeCommand(0xEA);                                                           //
  writeData(0x00);
  writeData(0x00);

  writeCommand(ILI9341_PWCTR1);    //Power control
  writeData(0x23);   //VRH[5:0]

  writeCommand(ILI9341_PWCTR2);    //Power control
  writeData(0x10);   //SAP[2:0];BT[3:0]

  writeCommand(ILI9341_VMCTR1);    //VCM control
  writeData(0x3e); //
  writeData(0x28);

  writeCommand(ILI9341_VMCTR2);    //VCM control2
  writeData(0x86);  //--

  writeCommand(ILI9341_MADCTL);    // Memory Access Control
  writeData(0x58);//writeData(0x48);                                            //Note: Set Bit 5 to enable line wrap when page end is reached. Used for very fast writing of Bitmaps

  writeCommand(ILI9341_PIXFMT);     //Set pixel format
  writeData(0x55);                  //0x55= 16 Bit Pixel

  writeCommand(ILI9341_FRMCTR1);    //Frame Rate
  writeData(0x00);
  writeData(0x18);

  writeCommand(ILI9341_DFUNCTR);    // Display Function Control
  writeData(0x08);
  writeData(0x82);
  writeData(0x27);

  writeCommand(0xF2);               // 3Gamma Function Disable
  writeData(0x00);

  writeCommand(ILI9341_GAMMASET);    //Gamma curve selected
  writeData(0x01);

  writeCommand(ILI9341_GMCTRP1);    //Set Gamma positive correction
  writeData(0x0F);
  writeData(0x31);
  writeData(0x2B);
  writeData(0x0C);
  writeData(0x0E);
  writeData(0x08);
  writeData(0x4E);
  writeData(0xF1);
  writeData(0x37);
  writeData(0x07);
  writeData(0x10);
  writeData(0x03);
  writeData(0x0E);
  writeData(0x09);
  writeData(0x00);

  writeCommand(ILI9341_GMCTRN1);    //Set Gamma negative correction
  writeData(0x00);
  writeData(0x0E);
  writeData(0x14);
  writeData(0x03);
  writeData(0x11);
  writeData(0x07);
  writeData(0x31);
  writeData(0xC1);
  writeData(0x48);
  writeData(0x08);
  writeData(0x0F);
  writeData(0x0C);
  writeData(0x31);
  writeData(0x36);
  writeData(0x0F);

  writeCommand(ILI9341_SLPOUT);    //Exit Sleep
  spi_end();
  delay(120);
  spi_begin();
  writeCommand(ILI9341_DISPON);    //Display on
  spi_end();
};


void ILI9341_ESP32::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
  writeCommand(ILI9341_CASET); // Column addr set
  writeData((x0 >> 8));
  writeData(x0 & 0xFF);
  writeData((x1 >> 8));
  writeData(x1);    //X0 = XSTART, X1 = XEND

  writeCommand(ILI9341_PASET); // Row addr set
  writeData((y0 >> 8));
  writeData(y0 & 0xFF);
  writeData((y1 >> 8));
  writeData(y1);    //Y0 = YSTART, Y1 = YEND

  writeCommand(ILI9341_RAMWR); // write to RAM
};








void ILI9341_ESP32::drawPixel(uint16_t x, uint16_t y, uint16_t color){
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
  spi_begin();
  setAddrWindow(x,y,x+1,y+1);
  digitalWrite(pin_dc, HIGH);
  SPI.write16(color);
  spi_end();
};





void ILI9341_ESP32::drawPixels(size_t length){
  while(length>=32){
    SPI.writeBytes(buf, 64);
    length-=32;
  };
  if(length>0)SPI.writeBytes(buf, length*2);       //write rest if any
};


void ILI9341_ESP32::clearDisplay (){
  fillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, ILI9341_BLACK);
}


/**

--------------- drawing lines ---------------

**/


void ILI9341_ESP32::drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color){
  fillRect(x, y, 1, h, color);
};


void ILI9341_ESP32::drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color){
  fillRect(x, y, w, 1, color);
};


void ILI9341_ESP32::drawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2,  uint16_t color){

};


/**

--------------- drawing figures ---------------

**/


void ILI9341_ESP32::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
  for(uint8_t i=0; i<64; i+=2){                                                 //Fill buffer. We don't take care if the line is smaller than this buffe
      buf[i]=color>>8;                                                          //...maybe we should take care later
      buf[i+1]=color;                                                           //...and we should also try to avoid this boilerplate code
  }
  spi_begin();
  setAddrWindow(x,y,x+w-1,y+h-1);
  digitalWrite(pin_dc, HIGH);
  drawPixels(w*h);
  spi_end();
}


void ILI9341_ESP32::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
  for(uint8_t i=0; i<64; i+=2){                                                 //Fill buffer. We don't take care if the line is smaller than this buffe
      buf[i]=color>>8;                                                          //...maybe we should take care later
      buf[i+1]=color;                                                           //...and we should also try to avoid this boilerplate code
  }
  spi_begin();

  //draw top line
  setAddrWindow(x,y,x+w-1,y+1);
  digitalWrite(pin_dc, HIGH);
  drawPixels(w);

  //draw right line
  setAddrWindow(x+w,y+1,x+w+1,y+h-2);
  digitalWrite(pin_dc, HIGH);
  drawPixels(h);

  //draw bottom line
  setAddrWindow(x,y+h-2,x+w-1,y+1);
  digitalWrite(pin_dc, HIGH);
  drawPixels(w);

  //draw right line
  setAddrWindow(x,y+1,x+1,y+h-2);
  digitalWrite(pin_dc, HIGH);
  drawPixels(h);

  spi_end();
}



void ILI9341_ESP32::setRotation(uint8_t m) {

  spi_begin();
  writeCommand(ILI9341_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     writeData(MADCTL_MX | MADCTL_BGR);
     _width  = TFT_WIDTH;
     _height = TFT_HEIGHT;
     break;
   case 1:
     writeData(MADCTL_MV | MADCTL_BGR);
     _width  = TFT_HEIGHT;
     _height = TFT_WIDTH;
     break;
  case 2:
    writeData(MADCTL_MY | MADCTL_BGR);
     _width  = TFT_WIDTH;
     _height = TFT_HEIGHT;
    break;
   case 3:
     writeData(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     _width  = TFT_HEIGHT;
     _height = TFT_WIDTH;
     break;
  }
  spi_end();
}
