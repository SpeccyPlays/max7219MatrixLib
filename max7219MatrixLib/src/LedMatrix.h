//Max7219LedLib
#ifndef LEDMATRIX_H
#define LEDMATRIX_H
#include <Arduino.h>
#include <SPI.h>
//below are the angles (in degrees) -180 to 150 in 30 degree steps
//they were converted to radians before sin(angle) and cos(angle)
const float cosValues[12] PROGMEM{-1.00,-0.87,-0.50,-0.00,0.50,0.87,1.00,0.87,0.50,-0.00,-0.50,-0.87};
const float sinValues[12] PROGMEM{0.00,-0.50,-0.87,-1.00,-0.87,-0.50,0.00,0.50,0.87,1.00,0.87, 0.50};
class LedMatrix {
    public:
    LedMatrix(uint8_t noOfModulesWide, uint8_t noOfModulesHigh);
    //matrix functions
    void init();
    void drawPixel(byte x, byte y);
    void wipeScreenBuffer();
    void sendScreenBuffer();
    void updateAll(uint16_t cmd, uint8_t data);
    void wipeDisplays();
    //basic drawing functions
    void plotLine(byte x1, byte y1, byte x2, byte y2);
    void plotCircle(byte xm, byte ym, uint8_t r);
    void makePointsThick(byte newX, byte newY);
    void plotCircleThick(byte xm, byte ym, uint8_t r);
    void plotSquare(byte x, byte y, byte width, byte height);
    void plotFilledSquare(byte x, byte y, byte width, byte height);
    //basic array drawing functions
    void draw8ColArray(byte xStart, byte yStart, const byte *array);
    void draw16ColArray(byte xStart, byte yStart, const byte *array);
    void drawCustomColArray(byte xStart, byte yStart, const byte *array, uint16_t startAt, byte chunkSize);
    //letter functions
    void drawLetter(byte xStart, byte yStart, char letter);
    void drawRotatedLetter(byte xStart, byte yStart, char letter, byte rotationValue);
    void drawRotatedLetter(byte xStart, byte yStart, float originX, float originY, char letter, byte rotationValue);
    void drawScaleLetter(byte xStart, byte yStart, float scaleX, float scaleY, char letter);
    void drawScaleAndRotatedLetter(byte xStart, byte yStart, float scaleX, float scaleY, byte rotationValue, char letter);
    //mirrored array functions
    void drawMirror8ColArray(byte xStart, byte yStart, const byte *array);
    void drawMirror16ColArray(byte xStart, byte yStart, const byte *array);
    void drawMirrorCustomColArray(byte xStart, byte yStart, const byte *array, uint16_t startAt, byte chunkSize);
    //functions for getting rotated x and y values
    float calcRotatedX(float x, float y, byte rotationValue);
    float calcRotatedY(float x, float y, byte rotationValue);
    //Rotation functions
    void drawRotated8ColArray(byte xStart, byte yStart, byte const *array, byte rotationValue);
    void drawMirrorRotated8ColArray(byte xStart, byte yStart, byte const *array, byte rotationValue);
    void drawRotated8ColArray(byte xStart, byte yStart, float originX, float originY, const byte *array, byte rotationValue);
    void drawMirrorRotated8ColArray(byte xStart, byte yStart, float originX, float originY, const byte *array, byte rotationValue);
    void drawRotated16ColArray(byte xStart, byte yStart, const byte *array, byte rotationValue);
    void drawRotated16ColArray(byte xStart, byte yStart, float originX, float originY, const byte *array, byte rotationValue);
    void drawRotatedCustomColArray(byte xStart, byte yStart, float originX, float originY, byte rotationValue, const byte *array, uint16_t startAt, byte chunkSize);
    void drawMirrorRotatedCustomColArray(byte xStart, byte yStart, float originX, float originY, byte rotationValue, const byte *array, uint16_t startAt, byte chunkSize);
    // function for getting scale values
    float scaleValue(byte value, float scaleValue);
    //scaled array functions
    void drawScale8ColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array);
    void drawScale16ColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array);
    void drawScaleCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array, uint16_t startAt, byte chunkSize);
    void drawMirrorScaleCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array, uint16_t startAt, byte chunkSize);
    //The functions that do it all
    void drawScaleAndRotatedCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, float originX, float originY, byte rotationValue, const byte *array, uint16_t startAt, byte chunkSize);
    void drawMirrorScaleAndRotatedCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, float originX, float originY, byte rotationValue, const byte *array, uint16_t startAt, byte chunkSize);
    private:
    //below are the command functions for the MAX7219
    //only the bare minimum is included
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