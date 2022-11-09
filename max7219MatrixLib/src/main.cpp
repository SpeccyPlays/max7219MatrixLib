#include <Arduino.h>
#include "LedMatrix.h"

/*
Working on z axis correctly. But sending the same to each group of displays
Think it's the way I'm sending them
*/

LedMatrix matrix(4, 3);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  matrix.init();
}

void loop() {
  for (byte i = 0; i < 32;i ++){
    matrix.wipeScreenBuffer();
  matrix.drawPixel(i, 0);
  matrix.sendScreenBuffer();
  delay(1000);
}
  // put your main code here, to run repeatedly:
  /*matrix.wipeScreenBuffer();
  for (uint8_t count = 0; count < 32; count +=4){
    matrix.plotLine(16, 0, count, 7);
    matrix.sendScreenBuffer();
    delay(100);
  }
  matrix.wipeScreenBuffer();
  for (byte count = 0; count < 10; count ++){
    matrix.plotCircle(random(0, 31), random(0, 7), random(2, 7));
    matrix.sendScreenBuffer();
    //matrix.wipeScreenBuffer();
    delay(100);
  }
  matrix.wipeScreenBuffer();
  for (byte count = 0; count < 10; count ++){
    matrix.plotSquare(random(0, 31), random(0, 7), random(2, 32), random(2, 7));
    matrix.sendScreenBuffer();
    //matrix.wipeScreenBuffer();
    delay(100);
  }*/
}