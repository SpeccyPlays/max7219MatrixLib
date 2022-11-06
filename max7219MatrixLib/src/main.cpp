#include <Arduino.h>
#include "LedMatrix.h"
LedMatrix matrix(4, 1);
void setup() {
  // put your setup code here, to run once:
  matrix.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  matrix.wipeScreenBuffer();
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
  }
}