#include <Arduino.h>
#include "LedMatrix.h"



LedMatrix matrix(4, 3);
uint16_t delayValue = 10;
byte count = 0;

byte willySprite[16]{
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
byte trumpetFacel[16]{
		B00001111, B00011111, B00111100, B00011100, B10001111, B01011111, B11111111, B01011111,
		B10011111, B00001111, B00001111, B10011100, B01111000, B00110000, B00010000, B00001000
}; //bottom left
byte trumpetFacer[16]{
		B00001000, B10011000, B11111000, B11111000, B10011000, B11101000, B11110000, B10000000,
		B11110000, B11100000, B11100000, B11100010, B01110100, B00111000, B00010000, B00100000 //bottom right
};
byte trumpetFace2l[16]{
		B00001111, B00011111, B00111100, B00011100, B00001111, B00011111, B11111111, B00011111,
		B00011111, B00001111, B00001111, B00000011, B00000011, B00000011, B00000011, B00000111
}; //bottom left
byte trumpetFace2r[16]{
		B00010000, B10110000, B11110000, B11110000, B10110000, B10010000, B11100000, B11110000,
		B11110000, B11100000, B11100000, B00000000, B00000000, B00000000, B00000000, B11000000 //bottom  right
};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  matrix.init();
}

void loop() {
  delayValue = 50;
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
  if (count == 0){
      matrix.draw16BitArray(4, 0, trumpetFacel);
    matrix.draw16BitArray(12, 0, trumpetFacer);
    count ++;
  }
  else {
    matrix.draw16BitArray(4, 0, trumpetFace2l);
    matrix.draw16BitArray(12, 0, trumpetFace2r);
    count = 0;
  }

  matrix.draw16BitArray(14, 8, willySprite);
  matrix.sendScreenBuffer();
  delayValue = 200;
  delay(delayValue);
}