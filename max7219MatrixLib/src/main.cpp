#include <Arduino.h>
#include "LedMatrix.h"

/*
Working on z axis correctly. But sending the same to each group of displays
Think it's the way I'm sending them
*/

LedMatrix matrix(4, 3);
uint16_t delayValue = 500;

byte willyHead[16]{
	B00000110, B00111110, B01111100, B00110100, B00111110, B00111100, B00011000, B00111100,
  B01111110, B01101110, B01101110, B01110110, B00111100, B00011000, B00011000, B00011100 //legs standing //head
}; 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  matrix.init();
}

void loop() {
  matrix.wipeScreenBuffer();
  /*for (uint8_t count = 0; count < 32; count +=8){
    matrix.plotLine(16, 12, count, 23);
    matrix.sendScreenBuffer();
    delay(delayValue);
  }
  for (uint8_t count = 24; count > 0; count -=8){
    matrix.plotLine(16, 12, 32, count);
    matrix.sendScreenBuffer();
    delay(delayValue);
  }
  for (uint8_t count = 32; count > 0; count -=8){
    matrix.plotLine(16, 12, count, 0);
    matrix.sendScreenBuffer();
    delay(delayValue);
  }
  for (uint8_t count = 0; count < 24; count +=8){
    matrix.plotLine(16, 12, 0, count);
    matrix.sendScreenBuffer();
    delay(delayValue);
  }
  matrix.wipeScreenBuffer();
  for (byte count = 0; count < 20; count ++){
    matrix.plotCircle(random(0, 31), random(0, 23), random(2, 7));
    matrix.sendScreenBuffer();
    //matrix.wipeScreenBuffer();
    delay(delayValue);
  }
  matrix.wipeScreenBuffer();
  for (byte count = 0; count < 20; count ++){
    matrix.plotSquare(random(0, 31), random(0, 23), random(2, 32), random(2, 23));
    matrix.sendScreenBuffer();
    //matrix.wipeScreenBuffer();
    delay(delayValue);
  }*/
  matrix.wipeScreenBuffer();
  matrix.drawPixel(0, 0);
  matrix.drawPixel(30, 0);
  matrix.draw16BitArray(12, 2, willyHead);
  matrix.sendScreenBuffer();
  delay(delayValue);
}