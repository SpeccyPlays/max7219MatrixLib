/*
This library is for the Max7219 controlled single colour Led Matrixes
As they can be daisy chained, I created this library to be able to quickly send data to them
and to keep the library size small.
I also created it to be able to use x, y co-ordinates like you would a TFT screen
X -> left to right, with 0 on the left
Y -> top to bottom with 0 at the top

It uses standard Aruino Uno SPI pins :
10 - CS
11 - MOSI
13 - CLK

The first matrix will need to be connected to the Arduino so the matrix input is on the (bottom) right hand side
Any other matrixes will need to be above this
Only works with squares, i.e, not possible to have a row with 3 modules then a row with 2 modules

Any sprite arrays have to be stored on flash using PROGMEM

Rotation values are 0-11 as precalculated sin and cos values are used for degrees -180 to 150 in 30 degree steps

*/

#include "LedMatrix.h"
#include <Arduino.h>
#include <SPI.h>

LedMatrix::LedMatrix(uint8_t noOfModulesWide, uint8_t noOfModulesHigh){
    this -> numOfModulesWide = noOfModulesWide;
    this -> numOfModulesHigh = noOfModulesHigh;
	screenBuffer = new byte[(noOfModulesWide * noOfModulesHigh) * ROWWIDTH];
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
		uint8_t moduleX = int(floor(x / 8));
		uint8_t moduleY = int(floor(y / 8));
		uint8_t arrayPosition = (moduleX * ROWWIDTH) + (moduleY * COLHEIGHT * numOfModulesWide) + int(y % 8);
		screenBuffer[arrayPosition] = screenBuffer[arrayPosition] | (temp >> int(x % 8));
	}
}
void LedMatrix::wipeScreenBuffer(){
  //zero the whole screen buffer
	for (byte i = 0; i < numOfModulesWide * numOfModulesHigh; i++){
    	for (byte j = 0; j < COLHEIGHT; j++){
      		screenBuffer[(i * ROWWIDTH) + j] = 0;
	  	}
	}
}
void LedMatrix::sendScreenBuffer(){
  //updates the matrixes with the screen buffer contents
	for (byte j = 0; j < COLHEIGHT; j++){
  		SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
		digitalWrite(CSPIN, LOW);
			for (byte i = 0; i < numOfModulesWide * numOfModulesHigh; i++){
				uint16_t temp = (j + 1) << 8 | screenBuffer[(i * ROWWIDTH) + j];
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
	for (byte i = 0; i < numOfModulesWide * numOfModulesHigh; i++){
	  SPI.transfer16(x);
	}
	digitalWrite(CSPIN, HIGH);
	SPI.endTransaction();
}
void LedMatrix::wipeDisplays(){
	//note when sending column data it goes from 1-8 as a 0 means no operation
	//This zeros the displays memory, rather than the buffer in this code
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
		makePointsThick(newX, newY);
		newX = xm-x;
		newY = ym+y;
		makePointsThick(newX, newY);
		newX = xm+x;
		newY = ym-y;
		makePointsThick(newX, newY);
		newX = xm-x;
		newY = ym-y;
		makePointsThick(newX, newY);
		newX = xm+y;
		newY = ym+x;
		makePointsThick(newX, newY);
		newX = xm-y;
		newY = ym+x;
		makePointsThick(newX, newY);
		newX = xm+y;
		newY = ym-x;
		makePointsThick(newX, newY);
		newX = xm-y;
		newY = ym-x;
		makePointsThick(newX, newY);
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
void LedMatrix::makePointsThick(byte newX, byte newY){
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
}
void LedMatrix::draw8ColArray(byte xStart, byte yStart, const byte *array){
	/*
	Draws a bitmap array stored in flash memory that is 8 columns high on the screen
	*/
	for (byte i = 0; i < COLHEIGHT; i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (array[i] & (128 >> j )){
				drawPixel(xStart + j, yStart + i);
			}
		}
	}
}
void LedMatrix::draw16ColArray(byte xStart, byte yStart, byte const *array){
	/*
	Draws a bitmap array stored in flash memory that is 16 columns high on the screen
	*/
	for (byte i = 0; i < (COLHEIGHT << 1); i++){ //use bitshift instead of * 2
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + j, yStart + i);
			}
		}
	}
}
void LedMatrix::drawCustomColArray(byte xStart, byte yStart, const byte *array, byte startAt, byte chunkSize){
	/*
	Draws a bitmap array stored in flash memory that is a user defined number of columns
	The start at is because I wanted to create a massive sprite in a 1d array and this allowed me to load chuncks in different positions
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	for (byte i = startAt; i < (startAt + chunkSize); i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + j, yStart + yCounter);
			}
		}
		yCounter++;
	}
}
float LedMatrix:: calcRotatedX(float x, float y, byte rotationValue){
	/*
	Seperate function as it's used many times
	*/
	float s = pgm_read_float_near(&sinValues[rotationValue]);
	float c = pgm_read_float_near(&cosValues[rotationValue]);
	return (x * c) - (y * s);
}
float LedMatrix:: calcRotatedY(float x, float y, byte rotationValue){
	/*
	Seperate function as it's used many times
	*/
	float s = pgm_read_float_near(&sinValues[rotationValue]);
	float c = pgm_read_float_near(&cosValues[rotationValue]);
	return (x * s) + (y * c);
}
void LedMatrix::drawRotated8ColArray(byte xStart, byte yStart, const byte *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 8 high
	Origin is fixed to center of sprite
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	if (rotationValue >= 0 && rotationValue < 12){
		for (byte i = 0; i < COLHEIGHT; i++){
			for (byte j = 0; j < ROWWIDTH; j++){
				if (pgm_read_byte(&array[i]) & (128 >> j )){
					float newX = calcRotatedX((xStart + j) - (xStart + 4.5), (yStart + i) - (yStart + 4.5), rotationValue);
					float newY = calcRotatedY((xStart + j) - (xStart + 4.5), (yStart + i) - (yStart + 4.5), rotationValue);
					drawPixel(int(newX + xStart + 4.5), int(newY + yStart + 4.5)); //round float and convert to int
				}
			}
		}
	}
}
void LedMatrix::drawRotated8ColArray(byte xStart, byte yStart, float originX, float originY, const byte *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 16 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 )
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	if (rotationValue >= 0 && rotationValue < 12){
		for (byte i = 0; i < COLHEIGHT; i++){
			for (byte j = 0; j < ROWWIDTH; j++){
				if (pgm_read_byte(&array[i]) & (128 >> j )){
					float newX = calcRotatedX((xStart + j) - (xStart + originX), (yStart + i) - (yStart + originY), rotationValue);
					float newY = calcRotatedY((xStart + j) - (xStart + originX), (yStart + i) - (yStart + originY), rotationValue);
					drawPixel(int(newX + xStart + originX), int(newY + yStart + originY)); //round float and convert to int
				}
			}
		}
	}
}
void LedMatrix::drawRotated16ColArray(byte xStart, byte yStart, const byte *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 16 high
	Origin is fixed to center of sprite
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	if (rotationValue >= 0 && rotationValue < 12){
		for (byte i = 0; i < (COLHEIGHT << 1); i++){ //bitshift to be fancy instead of * 2
			for (byte j = 0; j < ROWWIDTH; j++){
				if (pgm_read_byte(&array[i]) & (128 >> j )){
					float newX = calcRotatedX((xStart + j) - (xStart + 4.5), (yStart + i) - (yStart + 8.5), rotationValue);
					float newY = calcRotatedY((xStart + j) - (xStart + 4.5), (yStart + i) - (yStart + 8.5), rotationValue);
					drawPixel(int(newX + xStart + 4.5), int(newY + yStart + 8.5)); //round float and convert to int
				}
			}
		}
	}
}
void LedMatrix::drawRotated16ColArray(byte xStart, byte yStart, float originX, float originY, const byte *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 16 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 )
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	if (rotationValue >= 0 && rotationValue < 12){
		for (byte i = 0; i < (COLHEIGHT << 1); i++){ //bitshift to be fancy instead of * 2
			for (byte j = 0; j < ROWWIDTH; j++){
				if (pgm_read_byte(&array[i]) & (128 >> j )){
					float newX = calcRotatedX((xStart + j) - (xStart + originX), (yStart + i) - (yStart + originY), rotationValue);
					float newY = calcRotatedY((xStart + j) - (xStart + originX), (yStart + i) - (yStart + originY), rotationValue);
					drawPixel(int(newX + xStart + originX), int(newY + yStart + originY)); //round float and convert to int
				}
			}
		}
	}
}
void LedMatrix::drawRotatedCustomColArray(byte xStart, byte yStart, float originX, float originY, byte rotationValue, const byte *array, byte startAt, byte chunkSize){
	/*
	##untested##
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 16 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 )
	## Rotation values are 0-11 as precalculated sin and cos values are used ##*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	for (byte i = startAt; i < (startAt + chunkSize); i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				float newX = calcRotatedX((xStart + j) - (xStart + originX), (yStart + yCounter) - (yStart + originY), rotationValue);
				float newY = calcRotatedY((xStart + j) - (xStart + originX), (yStart + yCounter) - (yStart + originY), rotationValue);
				drawPixel(int(newX + xStart + originX), int(newY + yStart + originY));
			}
		}
		yCounter++;
	}
}
float LedMatrix::scaleValue(byte value, float scaleValue){
	return value * scaleValue;
}
void LedMatrix::drawScale8ColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array){
		/*
	Draws a scaled version of bitmap array stored in flash memory that is 8 columns high on the screen
	Use 1.0 for normal size
	*/
	for (byte i = 0; i < COLHEIGHT; i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + int(scaleValue(j, scaleX)), yStart + int(scaleValue(i, scaleY)));
			}
		}
	}	
}
void LedMatrix::drawScale16ColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array){
	/*
	Draws a scaled version of bitmap array stored in flash memory that is 16 columns high on the screen
	Use 1.0 for normal size
	*/
	for (byte i = 0; i < (COLHEIGHT << 1); i++){ //use bitshift instead of * 2 to be fancy
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + int(scaleValue(j, scaleX)), yStart + int(scaleValue(i, scaleY)));
			}
		}
	}
}
void LedMatrix::drawScaleCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array, byte startAt, byte chunkSize){
	/*
	Draws a bitmap array stored in flash memory that is a user defined number of columns
	The start at is because I wanted to create a massive sprite in a 1d array and this allowed me to load chuncks in different positions
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	for (byte i = startAt; i < (startAt + chunkSize); i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + int(scaleValue(j, scaleX)), yStart + int(scaleValue(yCounter, scaleY)));
			}
		}
		yCounter++;
	}
}
void LedMatrix::drawScaleAndRotatedCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, float originX, float originY, byte rotationValue, const byte *array, byte startAt, byte chunkSize){
	/*
	Draws a bitmap array stored in flash memory that is a user defined number of columns
	Can be both rotated and scaled
	The start at is because I wanted to create a massive sprite in a 1d array and this allowed me to load chuncks in different positions
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	for (byte i = startAt; i < (startAt + chunkSize); i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				//although it will take up stack, it's mentally easier to do the scaling seperate
				float scaleJ = scaleValue(j, scaleY);
				float scaleYCounter = scaleValue(yCounter, scaleY);
				float scaleOriginX = scaleValue(originX, scaleX );
				float scaleOriginY = scaleValue(originY, scaleY );
				float newX = calcRotatedX((xStart + scaleJ) - (xStart + scaleOriginX), (yStart + scaleYCounter) - (yStart + scaleOriginY), rotationValue);
				float newY = calcRotatedY((xStart + scaleJ) - (xStart + scaleOriginX), (yStart + scaleYCounter) - (yStart + scaleOriginY), rotationValue);
				drawPixel(int(newX + xStart + scaleOriginX), int(newY + yStart + scaleOriginY));
			}
		}
		yCounter++;
	}
}
;