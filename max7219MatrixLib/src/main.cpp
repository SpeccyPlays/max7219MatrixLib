#include <Arduino.h>
#include "LedMatrix.h"
#include "sprites.h"



LedMatrix matrix(4, 3);
uint16_t delayValue = 10;
byte count = 0;
void graphicsDemo();
void manicMinerSprites();

;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  matrix.init();
}

void loop() {
  delayValue = 100;
  
  matrix.wipeScreenBuffer();
  for (byte x = 0; x <= 16; x += 8){
    matrix.drawCustomSizeArray(x, 0, megaMan1, x * 3, 24);
  }
  matrix.sendScreenBuffer();
  delay(delayValue);
  matrix.wipeScreenBuffer();
  for (byte x = 0; x <= 16; x += 8){
    matrix.drawCustomSizeArray(x, 0, megaMan2, x * 3, 24);
  }
  matrix.sendScreenBuffer();
  delay(delayValue);
  matrix.wipeScreenBuffer();
  for (byte x = 0; x <= 16; x += 8){
    matrix.drawCustomSizeArray(x, 0, megaMan3, x * 3, 24);
  }
  matrix.sendScreenBuffer();
  delay(delayValue);
}
void graphicsDemo(){
  matrix.wipeScreenBuffer();
  for (uint8_t count = 0; count < 32; count +=8){
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
  }
}
void manicMinerSprites(){
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