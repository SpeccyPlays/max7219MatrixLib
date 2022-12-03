#include <Arduino.h>
#include "LedMatrix.h"
#include "sprites.h"
LedMatrix matrix(4, 3);
uint16_t delayValue = 100;
byte count = 0;
void graphicsDemo();
void customSpriteDemo();
void spriteRotationDemo();
const byte *megaMan[6] ={megaMan1, megaMan2, megaMan3, megaMan4, megaMan5, megaMan6};
void setup() {
  Serial.begin(115200);
  matrix.init();
  //graphicsDemo();
  //customSpriteDemo();
  //spriteRotationDemo();
}
void loop() {
  for (float i = 4.0; i >= -4.0; i -= 0.1){
    matrix.wipeScreenBuffer();
    matrix.drawScale8ColArray(16, 8, i, 1.0, willySprite);
    matrix.sendScreenBuffer();
    delay(delayValue - 50);
  }
  delay(delayValue);
  for (float i = 4.0; i >= -2.0; i -= 0.1){
    matrix.wipeScreenBuffer();
    matrix.drawScale8ColArray(12, 8, 1.0, i, willySprite);
    matrix.sendScreenBuffer();
    delay(delayValue - 50);
  }
  delay(delayValue);
}
void spriteRotationDemo(){
  byte counter = 0;
  for (byte j = 0; j < 3; j++){
    for (int16_t i = 0; i < 12; i++){
      matrix.wipeScreenBuffer();
      matrix.drawRotated16ColArray(counter, 4, willySprite, i);
      matrix.sendScreenBuffer();
      counter ++;
      delay(delayValue);
    }
  }
};
void customSpriteDemo(){
    //go through all the sprites
  for (byte i = 0; i < sizeof(megaMan)/sizeof(megaMan[0]); i++){
    matrix.wipeScreenBuffer();
    for (byte x = 0; x <= 16; x += 8){
      //the arrays are 72 columns
      //megaman is 24 x 24 pixels
      //the arrays are displayed in 3 chunks of 24
      //the each chunk is 8 pixels across from the last
      matrix.drawCustomColArray(x + 4, 0, megaMan[i], x * 3, 24);
    }
    matrix.sendScreenBuffer();
    delay(delayValue);
  }
  //just go through the running sprites
  for (byte loop = 0; loop < 6; loop++){
    for (byte i = 3; i < sizeof(megaMan)/sizeof(megaMan[0]); i++){
      matrix.wipeScreenBuffer();
      for (byte x = 0; x <= 16; x += 8){
        matrix.drawCustomColArray(x + 4, 0, megaMan[i], x * 3, 24);
      }
      matrix.sendScreenBuffer();
      delay(delayValue);
    }
  }
  matrix.wipeScreenBuffer();
  for (byte x = 0; x <= 16; x += 8){
    matrix.drawCustomColArray(x + 4, 0, megaMan[2], x * 3, 24);
    }
  matrix.sendScreenBuffer();
  delay(delayValue);
}
void graphicsDemo(){
  /*
  Demo for the standard graphics type drawing stuff
  */
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