#include "LedMatrix.h"
#include <Arduino.h>
#include <SPI.h>

LedMatrix::LedMatrix(uint8_t noOfModulesWide, uint8_t noOfModulesHigh){
	//byte buffer[4][8]{0};
    //byte buffer[(noOfModulesWide + noOfModulesHigh) * ROWWIDTH]{0};
    this -> numOfModulesWide = noOfModulesWide;
    this -> numOfModulesHigh = noOfModulesHigh;
	//screenBuffer = buffer;
}
void LedMatrix::init(){
	pinMode(CSPIN, OUTPUT);
	SPI.begin();
  	wipeDisplays();
	updateAll(INTENSITY, 0);
	updateAll(DISPLAY_TEST, 0);
	updateAll(DECODE_MODE, 0);
	updateAll(SCAN_LIMIT, 7);
	updateAll(SHUTDOWN, 1);
}
void LedMatrix::drawPixel(byte x, byte y){
  //draw a pixel in screen memory
  	if ((x >= 0 && x < ( numOfModulesWide * ROWWIDTH)) && (y >= 0 && y < (numOfModulesHigh * COLHEIGHT))){
	uint8_t temp = 128; //set MSB
	uint8_t module = int(floor(x / 8));
	//screenBuffer[(module * ROWWIDTH) + y] = screenBuffer[(module * ROWWIDTH) + y] | (temp >> int(x % 8));

	screenBuffer[module][COLHEIGHT - y - 1] = screenBuffer[module][COLHEIGHT - y - 1] | (temp >> int(x % 8));
	}
}
void LedMatrix::wipeScreenBuffer(){
  //zero the whole screen buffer
	for (byte i = 0; i < numOfModulesWide; i++){
    	for (byte j = 0; j < COLHEIGHT; j++){
      		screenBuffer[i][j] = 0;
	  	}
	}
}
void LedMatrix::sendScreenBuffer(){
  //updates the matrixes with the screen buffer contents
	for (byte j = 0; j < COLHEIGHT; j++){
  		SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
		digitalWrite(CSPIN, LOW);
			for (byte i = 0; i < numOfModulesWide; i++){
				uint16_t temp = (j + 1) << 8 | screenBuffer[i][j];
    			SPI.transfer16(temp);
			}
		digitalWrite(CSPIN, HIGH);
		SPI.endTransaction();	
	}
}
void LedMatrix::updateAll(uint16_t cmd, uint8_t data){
//used for sending operation codes
	uint16_t x = (cmd << 8) | data;
	SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
	digitalWrite(CSPIN, LOW);
	for (byte i = 0; i < numOfModulesWide; i++){
	  SPI.transfer16(x);
	}
	digitalWrite(CSPIN, HIGH);
	SPI.endTransaction();
}
void LedMatrix::wipeDisplays(){
  for (byte colNumber = 1; colNumber <= COLHEIGHT; colNumber++){
    updateAll(colNumber, 0);
  }
}
void LedMatrix::plotLine(byte x1, byte y1, byte x2, byte y2){
// Bresenham's Line Drawing Algorithm
// shamelessly copied and pasted from stackoverflow
	int x;int y;int dx;int dy;int dx1;int dy1;int px;int py;int xe;int ye;int i;
	dx=x2-x1;dy=y2-y1;
	dx1=fabs(dx);dy1=fabs(dy); 
	px=2*dy1-dx1; py=2*dx1-dy1; 
	if(dy1<=dx1) { 
		if(dx>=0) { x=x1; y=y1; xe=x2; 
			} 
		else { x=x2; y=y2; xe=x1; 
			} 
		drawPixel(x,y); 
		for(i=0;x<xe;i++) { 
			x=x+1; 
			if(px<0) { 
				px=px+2*dy1; 
				} 
			else { 
				if((dx<0 && dy<0) || (dx>0 && dy>0)) { 
					y=y+1; 
					} 
				else { 
					y=y-1; 
					} 
				px=px+2*(dy1-dx1); 
				} 
//			delay(0); 
			drawPixel(x,y); 
			} 
		} 
	else { 
		if(dy>=0) { x=x1; y=y1; ye=y2; 
			} 
		else { x=x2; y=y2; ye=y1; 
			} 
		drawPixel(x,y); 
		for(i=0;y<ye;i++) { y=y+1; 
			if(py<=0) { py=py+2*dx1; 
				} 
			else { 
				if((dx<0 && dy<0) || (dx>0 && dy>0)) { x=x+1; 
					} 
				else { x=x-1; 
					} 
				py=py+2*(dx1-dy1); 
				} 
//			delay(0); 
			drawPixel(x,y); 
			} 
		} 
}
void LedMatrix::plotCircleThick(byte xm, byte ym, uint8_t r){
//makes a thicker, better looking circle
// another copy and paste algorithum
// There's no real need for the newX or newY but left them in
	int x = 0; 
	int y = r; 
	int d = 3-2*r;
	while (y >= x){
		byte newX = xm+x;
		byte newY = ym+y;
		makePointsThicc(newX, newY);
		newX = xm-x;
		newY = ym+y;
		makePointsThicc(newX, newY);
		newX = xm+x;
		newY = ym-y;
		makePointsThicc(newX, newY);
		newX = xm-x;
		newY = ym-y;
		makePointsThicc(newX, newY);
		newX = xm+y;
		newY = ym+x;
		makePointsThicc(newX, newY);
		newX = xm-y;
		newY = ym+x;
		makePointsThicc(newX, newY);
		newX = xm+y;
		newY = ym-x;
		makePointsThicc(newX, newY);
		newX = xm-y;
		newY = ym-x;
		makePointsThicc(newX, newY);
		x++;
		if (d > 0){
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else {
			d = d + 4 * x + 6;
		}
	} 
}
void LedMatrix::makePointsThicc(byte newX, byte newY){
// Draws all the way around a point
	drawPixel(newX, newY);
	drawPixel(newX - 1, newY);
	drawPixel(newX, newY - 1);
	drawPixel(newX - 1, newY - 1);
	drawPixel(newX + 1, newY - 1);
	drawPixel(newX + 1, newY);
	drawPixel(newX + 1, newY + 1);
	drawPixel(newX, newY + 1);
	drawPixel(newX - 1, newY + 1);		
}
void LedMatrix::plotCircle(byte xm, byte ym, uint8_t r){
// another copy and paste algorithum
	int x = 0; 
	int y = r; 
	int d = 3-2*r;
	while (y >= x){
		drawPixel(xm+x, ym+y);
		drawPixel(xm-x, ym+y);
		drawPixel(xm+x, ym-y);
		drawPixel(xm-x, ym-y);
		drawPixel(xm+y, ym+x);
		drawPixel(xm-y, ym+x);
		drawPixel(xm+y, ym-x);
		drawPixel(xm-y, ym-x);		
		x++;
		if (d > 0){
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else {
			d = d + 4 * x + 6;
		}
	} 
}
void LedMatrix::plotSquare(byte x, byte y, byte width, byte height){
	plotLine(x, y, x + width, y);
	plotLine(x + width, y, x + width, y + height);
	plotLine(x + width, y + height, x, y + height);
	plotLine(x, y + height, x, y);
}
void LedMatrix::plotFilledSquare(byte x, byte y, byte width, byte height){
	for (byte i = 0; i < height; i++){
		for (byte j = 0; j < width; j++){
			drawPixel(x + j, y + i);
		}
	}
};