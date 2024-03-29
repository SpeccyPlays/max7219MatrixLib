#include <Arduino.h>
#include "LedMatrix.h"
#include "sprites.h"
#include "Fonts.h"
LedMatrix matrix(4, 3);
//used for demos
uint16_t delayValue = 100;
byte count = 0;
byte xCounter = 0;
void graphicsDemo();
void customSpriteDemo();
void spriteScalingDemo();
void letterRotationDemo();
void spriteRotationDemo();
void spriteScalingAndRotatingDemo();
void penguinAnimation();
void letterSkewedDemo();
const byte *megaMan[6] ={megaMan1, megaMan2, megaMan3, megaMan4, megaMan5, megaMan6};
void setup() {
  randomSeed(A0);
  Serial.begin(115200);
  matrix.init();
}
void loop() {
  /*penguinAnimation();
  graphicsDemo();
  customSpriteDemo();
  spriteScalingDemo();
  spriteRotationDemo();
  spriteScalingAndRotatingDemo();
  letterRotationDemo();*/
  letterSkewedDemo();
  //delay(delayValue + 100);

}
void letterSkewedDemo(){
  for (int8_t movement = 32; movement > -58; movement --){
    matrix.wipeScreenBuffer();
    matrix.drawSkewLetter(movement, 4, 1, 0, 'S');
    matrix.drawSkewLetter(movement + 10, 4, 1, 0, 'P');
    matrix.drawSkewLetter(movement + 20, 4, 1, 0, 'E');
    matrix.drawSkewLetter(movement + 30, 4, 1, 0, 'C');
    matrix.drawSkewLetter(movement + 40, 4, 1, 0, 'C');
    matrix.drawSkewLetter(movement + 50, 4, 1, 0, 'Y');
    matrix.drawSkewLetter(movement + 10, 12, 1, 0, 'P');
    matrix.drawSkewLetter(movement + 20, 12, 1, 0, 'L');
    matrix.drawSkewLetter(movement + 30, 12, 1, 0, 'A');
    matrix.drawSkewLetter(movement + 40, 12, 1, 0, 'Y');
    matrix.drawSkewLetter(movement + 50, 12, 1, 0, 'S');
    matrix.sendScreenBuffer();
    delay(30);

  }
}
void penguinAnimation(){
  //trying a tiny Lemmings style penguin sprite
  for (byte i = 0; i < sizeof(penguin)/sizeof(penguin[0]); i += 9){
    matrix.wipeScreenBuffer();
    matrix.drawCustomColArray(xCounter, 15, penguin, i, 9);
    matrix.sendScreenBuffer();
    delay(delayValue);
    xCounter ++;
    if (xCounter > 31){
      xCounter = 0;
    }
  }
}
void spriteScalingAndRotatingDemo(){
  for (float i = 2.00; i >= 0.00; i -= 0.1){
    for (byte rot = 0; rot < 12; rot++){
      matrix.wipeScreenBuffer();
      for (byte x = 0; x <= 16; x += 8){
      //the arrays are 72 columns
      //megaman is 24 x 24 pixels
      //the arrays are displayed in 3 chunks of 24
      //the each chunk is 8 pixels across from the last
        matrix.drawScaleAndRotatedCustomColArray(x + 4, 0, i, i, 14 - x, 11, rot, megaMan1, x * 3, 24);
      }
      matrix.sendScreenBuffer();
      matrix.wipeScreenBuffer();
      delay(delayValue -50 );
    }
  }
}
void spriteScalingDemo(){
  delay(delayValue);
  for (float i = 3.0; i >= 0.0; i -= 0.1){
    matrix.wipeScreenBuffer();
    for (byte x = 0; x <= 16; x += 8){
      //the arrays are 72 columns
      //megaman is 24 x 24 pixels
      //the arrays are displayed in 3 chunks of 24
      //the each chunk is 8 pixels across from the last
        matrix.drawScaleCustomColArray(x + 4, 0, i, i, megaMan1, x * 3, 24);
    }
    matrix.sendScreenBuffer();
    matrix.wipeScreenBuffer();
    delay(delayValue+50);
  }
  delay(300);
}
void letterRotationDemo(){
  matrix.wipeScreenBuffer();
  matrix.drawLetter(0, 0,'S');
  matrix.drawLetter(8, 0, 'P');
  matrix.drawLetter(16, 0, 'E');
  matrix.drawLetter(24, 0, 'C');
  matrix.drawLetter(0, 8, 'C');
  matrix.drawLetter(8, 8, 'Y');
  matrix.drawLetter(16, 8, ' ');
  matrix.drawLetter(24, 8, 'P');
  matrix.drawLetter(0, 16, 'L');
  matrix.drawLetter(8, 16, 'A');
  matrix.drawLetter(16, 16, 'Y');
  matrix.drawLetter(24, 16, 'S');
  matrix.sendScreenBuffer();
  delay(delayValue+200);
  for (byte i = 0; i < 12; i++){
    matrix.wipeScreenBuffer();
    matrix.drawRotatedLetter(0, 0, 'S', 11-i);
    matrix.drawRotatedLetter(8, 0, 'P', 11-i);
    matrix.drawRotatedLetter(16, 0, 'E', 11-i);
    matrix.drawRotatedLetter(24, 0, 'C', 11-i);
    matrix.drawRotatedLetter(0, 8, 'C', 11-i);
    matrix.drawRotatedLetter(8, 8, 'Y', 11-i);
    matrix.drawRotatedLetter(16, 8, ' ', 11-i);
    matrix.drawRotatedLetter(24, 8, 'P', 11-i);
    matrix.drawRotatedLetter(0, 16, 'L', 11-i);
    matrix.drawRotatedLetter(8, 16, 'A', 11-i);
    matrix.drawRotatedLetter(16, 16, 'Y', 11-i);
    matrix.drawRotatedLetter(24, 16, 'S', 11-i);
    matrix.sendScreenBuffer();
    //delay(delayValue);
  }
};
void spriteRotationDemo(){
  byte counter = 0;
  for (byte j = 0; j < 3; j++){
    for (byte i = 0; i < 12; i++){
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