//Max7219LedLib
#ifndef LEDMATRIX_H
#define LEDMATRIX_H
#include <Arduino.h>
#include <SPI.h>
class LedMatrix {
    public:
    LedMatrix(uint8_t noOfModulesWide, uint8_t noOfModulesHigh);
    void init();
    void drawPixel(byte x, byte y);
    void wipeScreenBuffer();
    void sendScreenBuffer();
    void updateAll(uint16_t cmd, uint8_t data);
    void wipeDisplays();
    void plotLine(byte x1, byte y1, byte x2, byte y2);
    void plotCircle(byte xm, byte ym, uint8_t r);
    void makePointsThicc(byte newX, byte newY);
    void plotCircleThick(byte xm, byte ym, uint8_t r);
    void plotSquare(byte x, byte y, byte width, byte height);
    void plotFilledSquare(byte x, byte y, byte width, byte height);
    void draw8BitArray(byte xStart, byte yStart, byte array[]);
    void draw16BitArray(byte xStart, byte yStart, byte array[]);
    private:
    const uint8_t NO_OP = 0x00;
    const uint8_t DECODE_MODE = 9;
    const uint8_t INTENSITY = 10;
    const uint8_t SCAN_LIMIT = 11;
    const uint8_t SHUTDOWN = 12;
    const uint8_t DISPLAY_TEST = 15;
    //CS pin
    const uint8_t CSPIN = 10;
    //LED Matrix details 
    byte numOfModulesWide = 1;
    byte numOfModulesHigh = 1;
    const byte ROWWIDTH = 8;
    const byte COLHEIGHT = 8;
    byte *screenBuffer;
    };
#endif