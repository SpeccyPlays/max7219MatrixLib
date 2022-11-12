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
byte worm1[16]{
	B00111111, B01000000, B10000000, B10010100, B10010100, B10010100, B10010100, B1000000,
  B01000010, B01111100, B10000001, B10000001, B10000001, B10000000, B10000000, B1111111 
};
byte worm2[16]{
	B00000000, B10000000, B01000000, B01000000, B01000000, B01000000, B01000000, B01000000,
  B10000000, B10000000, B00000000, B01100000, B10010000, B00001000, B00100100, B11011000 
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
  matrix.draw16BitArray(23, 0, willyHead);
  matrix.draw16BitArray(0, 0, worm1);
  matrix.draw16BitArray(8, 0, worm2);
  matrix.sendScreenBuffer();
  delay(delayValue);
}