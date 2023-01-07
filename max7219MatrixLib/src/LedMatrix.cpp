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
#include "Fonts.h"

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
	wipeScreenBuffer();
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
uint8_t LedMatrix::checkCharInFontArray(char letter){
	/*
	Checks the letter provided has a character in the font map
	Starts at space and ends at ~ which are 32 (U+0020) and 126 (U+007E) respectively
	Control characters are included in the array but excluded from being drawn
	*/
	if (int(letter) >= 32 && int(letter) < 127 ){
		return 1;
	}
	else return 0;
}
uint8_t LedMatrix::checkRotationValueAllowed(uint8_t rotationValue){
	/*
	Check the rotation value is between 0 and 11 as this is the amount of precalcuated values
	*/
	if (rotationValue >= 0 && rotationValue < 12){
		return 1;
	}
	else return 0;
}
void LedMatrix::draw8ColArray(byte xStart, byte yStart, const byte *array){
	/*
	Draws a bitmap array stored in flash memory that is 8 columns high on the screen
	*/
	drawCustomColArray(xStart, yStart, array, 0, 8);
}
void LedMatrix::draw16ColArray(byte xStart, byte yStart, byte const *array){
	/*
	Draws a bitmap array stored in flash memory that is 16 columns high on the screen
	*/
	drawCustomColArray(xStart, yStart, array, 0, 16);
}
void LedMatrix::drawCustomColArray(byte xStart, byte yStart, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draws a bitmap array stored in flash memory that is a user defined number of columns
	The start at is because I wanted to create a massive sprite in a 1d array and this allowed me to load chuncks in different positions
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + j, yStart + yCounter);
			}
		}
		yCounter++;
	}
}
void LedMatrix::drawLetter(byte xStart, byte yStart, char letter){
	/*
	Fonts are stored in reverse (choice of who I copied the array from) so need to mirror when drawing
	*/
	if (checkCharInFontArray(letter)){
		drawMirrorCustomColArray(xStart, yStart, font8x8_basic, int(letter) * COLHEIGHT, 8);
	}
}
void LedMatrix::drawRotatedLetter(byte xStart, byte yStart, char letter, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of character 8 pixels wide x 8 high
	Origin is fixed to center of sprite
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	//I don't know why 3 is the center point !
	if (checkRotationValueAllowed(rotationValue)){
		float originXandY = 3; //center of both x and y. Makes sure values are float as VSCode assumes they're a double otherwise
		drawMirrorRotatedCustomColArray(xStart, yStart, originXandY, originXandY, rotationValue, font8x8_basic, int(letter) * COLHEIGHT, 8);
	}
};
void LedMatrix::drawRotatedLetter(byte xStart, byte yStart, float originX, float originY, char letter, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of character 8 pixels wide x 8 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 ) ##update not sure origin is correct
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	if (checkRotationValueAllowed(rotationValue)){
		drawMirrorRotatedCustomColArray(xStart, yStart, originX, originY, rotationValue, font8x8_basic, int(letter) * COLHEIGHT, 8);
	}
};
void LedMatrix::drawScaleLetter(byte xStart, byte yStart, float scaleX, float scaleY, char letter){
		/*
	Draws a scaled version of letter stored in flash memory on the screen
	Use 1.0 for normal size
	*/
	if (checkCharInFontArray(letter)){
		drawMirrorScaleCustomColArray(xStart, yStart, scaleX, scaleY, font8x8_basic, int(letter) * COLHEIGHT, 8);
	}
}
void LedMatrix::drawScaleAndRotatedLetter(byte xStart, byte yStart, float scaleX, float scaleY, byte rotationValue, char letter){
	if (checkCharInFontArray(letter) && checkRotationValueAllowed(rotationValue)){
		drawMirrorScaleAndRotatedCustomColArray(xStart, yStart, scaleX, scaleY, 3, 3, rotationValue, font8x8_basic, int(letter) * COLHEIGHT, 8);
	}
}
void LedMatrix::drawSkewLetter(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, char letter){
	if (checkCharInFontArray(letter)){
		drawSkewMirrorCustomColArray(xStart, yStart, skewXValue, skewYValue, font8x8_basic, int(letter) * COLHEIGHT, 8);
	}
}
void LedMatrix::drawSkewAndScaleLetter(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, float scaleXValue, float scaleYValue, char letter){
	if (checkCharInFontArray(letter)){
		drawSkewAndScaleMirrorCustomColArray(xStart, yStart, skewXValue, skewYValue,  scaleXValue, scaleYValue, font8x8_basic, int(letter) * COLHEIGHT, 8);
	}
}
void LedMatrix::drawMirror8ColArray(byte xStart, byte yStart, const byte *array){
	/*
	Draw a horizontally mirrored 8x8 array
	*/
	drawMirrorCustomColArray(xStart, yStart, array, 0, 8);
}
void LedMatrix::drawMirror16ColArray(byte xStart, byte yStart, const byte *array){
	/*
	Draw a horizontally mirrored 8x16 array
	*/
	drawMirrorCustomColArray(xStart, yStart, array, 0, 16);
}
void LedMatrix::drawMirrorCustomColArray(byte xStart, byte yStart, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draw horizontally mirrored custom column sized array
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (1 << j )){
				drawPixel(xStart + j, yStart + yCounter);
			}
		}
		yCounter++;
	}
}
void LedMatrix::drawSkew8ColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, const byte *array){
	drawSkewCustomColArray(xStart, yStart, skewXValue, skewYValue,array, 0, 8);
}
void LedMatrix::drawSkew16ColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, const byte *array){
	drawSkewCustomColArray(xStart, yStart, skewXValue, skewYValue, array, 0, 16);
}
void LedMatrix::drawSkewCustomColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draw an array that is skewed by the amount specified.
	Better if only skewing x or y, not both together
	Note, it is done by a pixel amount and cumulative as it moves through the amount of columns and rows
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	int8_t skewXCounter = 0;
	int8_t skewYCounter = 0;
	for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
		skewXCounter = yCounter * skewXValue;
		for (byte j = 0; j < ROWWIDTH; j++){
			skewYCounter = j * skewYValue;
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + j + skewXCounter, yStart + yCounter + skewYCounter);
			}
		}
		yCounter++;
	}
}

void LedMatrix::drawSkewMirror8ColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, const byte *array){
	drawSkewMirrorCustomColArray(xStart, yStart, skewXValue, skewYValue, array, 0, 8);
}
void LedMatrix::drawSkewMirror16ColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, const byte *array){
	drawSkewMirrorCustomColArray(xStart, yStart, skewXValue, skewYValue, array, 0, 16);
}
void LedMatrix::drawSkewMirrorCustomColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draw a mirrored array that is skewed by the amount specified.
	Better if only skewing x or y, not both together
	Note, it is done by a pixel amount and cumulative as it moves through the amount of columns and rows
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	int8_t skewXCounter = 0;
	int8_t skewYCounter = 0;
	for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
		skewXCounter = yCounter * skewXValue;
		for (byte j = 0; j < ROWWIDTH; j++){
			skewYCounter = j * skewYValue;
			if (pgm_read_byte(&array[i]) & (1 << j )){
				drawPixel(xStart + j + skewXCounter, yStart + yCounter + skewYCounter);
			}
		}
		yCounter++;
	}
}
void LedMatrix::drawSkewAndScale8ColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, float scaleXValue, float scaleYValue, const byte *array){
	drawSkewAndScaleCustomColArray(xStart, yStart, skewXValue, skewYValue, scaleXValue, scaleYValue, array, 0, 8);
}
void LedMatrix::drawSkewAndScale16ColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, float scaleXValue, float scaleYValue, const byte *array){
	drawSkewAndScaleCustomColArray(xStart, yStart, skewXValue, skewYValue, scaleXValue, scaleYValue,  array, 0, 16);
}
void LedMatrix::drawSkewAndScaleCustomColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, float scaleXValue, float scaleYValue, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draw an array that is skewed by the amount specified.
	Better if only skewing x or y, not both together
	Note, it is done by a pixel amount and cumulative as it moves through the amount of columns and rows
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	int8_t skewXCounter = 0;
	int8_t skewYCounter = 0;
	for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
		skewXCounter = yCounter * skewXValue;
		for (byte j = 0; j < ROWWIDTH; j++){
			skewYCounter = j * skewYValue;
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + int(scaleValue(j + skewXCounter, scaleXValue)), yStart + int(scaleValue(yCounter + skewYCounter, scaleYValue)));
			}
		}
		yCounter++;
	}
}
void LedMatrix::drawSkewAndScaleMirror8ColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, float scaleXValue, float scaleYValue, const byte *array){
	drawSkewAndScaleMirrorCustomColArray(xStart, yStart, skewXValue, skewYValue, scaleXValue, scaleYValue, array, 0, 8);
}
void LedMatrix::drawSkewAndScaleMirror16ColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, float scaleXValue, float scaleYValue, const byte *array){
	drawSkewAndScaleMirrorCustomColArray(xStart, yStart, skewXValue, skewYValue, scaleXValue, scaleYValue,  array, 0, 16);
}
void LedMatrix::drawSkewAndScaleMirrorCustomColArray(byte xStart, byte yStart, int8_t skewXValue, int8_t skewYValue, float scaleXValue, float scaleYValue, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draw an array that is skewed by the amount specified.
	Better if only skewing x or y, not both together
	Note, it is done by a pixel amount and cumulative as it moves through the amount of columns and rows
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	int8_t skewXCounter = 0;
	int8_t skewYCounter = 0;
	for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
		skewXCounter = yCounter * skewXValue;
		for (byte j = 0; j < ROWWIDTH; j++){
			skewYCounter = j * skewYValue;
			if (pgm_read_byte(&array[i]) & (1 << j )){
				drawPixel(xStart + int(scaleValue(j + skewXCounter, scaleXValue)), yStart + int(scaleValue(yCounter + skewYCounter, scaleYValue)));
			}
		}
		yCounter++;
	}
}
float LedMatrix::calcRotatedX(float x, float y, byte rotationValue){
	/*
	Calculates x value for 2d rotations
	*/
	float s = pgm_read_float_near(&sinValues[rotationValue]);
	float c = pgm_read_float_near(&cosValues[rotationValue]);
	return (x * c) - (y * s);
}
float LedMatrix:: calcRotatedY(float x, float y, byte rotationValue){
	/*
	Calculates y for 2d rotations
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
	float originXandY = 3; //center of both x and y. Makes sure values are float as VSCode assumes they're a double otherwise
	drawRotatedCustomColArray(xStart, yStart, originXandY, originXandY, rotationValue, array, 0, 8);
}
void LedMatrix::drawMirrorRotated8ColArray(byte xStart, byte yStart, byte const *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 8 high
	Array is mirrored horizontally
	Origin is fixed to center of sprite
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	float originXandY = 3; //center of both x and y. Makes sure values are float as VSCode assumes they're a double otherwise
	drawMirrorRotatedCustomColArray(xStart, yStart, originXandY, originXandY, rotationValue, array, 0, 8);
}
void LedMatrix::drawRotated8ColArray(byte xStart, byte yStart, float originX, float originY, const byte *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 8 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 )
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	drawRotatedCustomColArray(xStart, yStart, originX, originY, rotationValue, array, 0, 8);
}
void LedMatrix::drawMirrorRotated8ColArray(byte xStart, byte yStart, float originX, float originY, const byte *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 8 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 )
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	Shows Mirror version of array
	*/
	drawMirrorRotatedCustomColArray(xStart, yStart, originX, originY, rotationValue, array, 0, 8);
}
void LedMatrix::drawRotated16ColArray(byte xStart, byte yStart, const byte *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 16 high
	Origin is fixed to center of sprite
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	float originX = 3.5;
	float originY = 7.5;
	drawRotatedCustomColArray(xStart, yStart, originX, originY, rotationValue, array, 0, 16);
}
void LedMatrix::drawRotated16ColArray(byte xStart, byte yStart, float originX, float originY, const byte *array, byte rotationValue){
	/*
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 16 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 )
	## Rotation values are 0-11 as precalculated sin and cos values are used ##
	*/
	drawRotatedCustomColArray(xStart, yStart, originX, originY, rotationValue, array, 0, 16);
}
void LedMatrix::drawRotatedCustomColArray(byte xStart, byte yStart, float originX, float originY, byte rotationValue, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 16 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 )
	## Rotation values are 0-11 as precalculated sin and cos values are used ##*/
	if (checkRotationValueAllowed(rotationValue)){
		byte yCounter = 0; //used for y position as if we use i it can have massive values
		for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
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
}
void LedMatrix::drawMirrorRotatedCustomColArray(byte xStart, byte yStart, float originX, float originY, byte rotationValue, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Rotates an array mirrored horizontally
	Starts drawing at x, y position but rotates around center point of sprite 8 pixels wide x 16 high
	Allows the origin point to be choosen, relative to the x and y start points
	So to rotate around the center of a 8 wide 16 high sprite the originX will be 4.5 (halway point between 1 and 8)
	originY would be 8.5 (half way point between 1 and 16 )
	## Rotation values are 0-11 as precalculated sin and cos values are used ##*/
	if (checkRotationValueAllowed(rotationValue)){
		byte yCounter = 0; //used for y position as if we use i it can have massive values
		for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
			for (byte j = 0; j < ROWWIDTH; j++){
				if (pgm_read_byte(&array[i]) & (1 << j )){
					float newX = calcRotatedX((xStart + j) - (xStart + originX), (yStart + yCounter) - (yStart + originY), rotationValue);
					float newY = calcRotatedY((xStart + j) - (xStart + originX), (yStart + yCounter) - (yStart + originY), rotationValue);
					drawPixel(int(newX + xStart + originX), int(newY + yStart + originY));
				}
			}
			yCounter++;
		}
	}
}
float LedMatrix::scaleValue(int8_t value, float scaleValue){
	return value * scaleValue;
}
void LedMatrix::drawScale8ColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array){
		/*
	Draws a scaled version of bitmap array stored in flash memory that is 8 columns high on the screen
	Use 1.0 for normal size
	*/
	drawScaleCustomColArray(xStart, yStart, scaleX, scaleY, array, 0, 8);
}
void LedMatrix::drawScale16ColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array){
	/*
	Draws a scaled version of bitmap array stored in flash memory that is 16 columns high on the screen
	Use 1.0 for normal size
	*/
	drawScaleCustomColArray(xStart, yStart, scaleX, scaleY, array, 0, 16);
}
void LedMatrix::drawScaleCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draws a bitmap array stored in flash memory that is a user defined number of columns
	The start at is because I wanted to create a massive sprite in a 1d array and this allowed me to load chuncks in different positions
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (128 >> j )){
				drawPixel(xStart + int(scaleValue(j, scaleX)), yStart + int(scaleValue(yCounter, scaleY)));
			}
		}
		yCounter++;
	}
}
void LedMatrix::drawMirrorScaleCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draws a bitmap array stored in flash memory that is a user defined number of columns
	The start at is because I wanted to create a massive sprite in a 1d array and this allowed me to load chuncks in different positions
	*/
	byte yCounter = 0; //used for y position as if we use i it can have massive values
	for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			if (pgm_read_byte(&array[i]) & (1 << j )){
				drawPixel(xStart + int(scaleValue(j, scaleX)), yStart + int(scaleValue(yCounter, scaleY)));
			}
		}
		yCounter++;
	}
}
void LedMatrix::drawScaleAndRotatedCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, float originX, float originY, byte rotationValue, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draws a bitmap array stored in flash memory that is a user defined number of columns
	Can be both rotated and scaled
	The start at is because I wanted to create a massive sprite in a 1d array and this allowed me to load chuncks in different positions
	*/
	if (checkRotationValueAllowed(rotationValue)){
		byte yCounter = 0; //used for y position as if we use i it can have massive values
		for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
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
}
void LedMatrix::drawMirrorScaleAndRotatedCustomColArray(byte xStart, byte yStart, float scaleX, float scaleY, float originX, float originY, byte rotationValue, const byte *array, uint16_t startAt, byte chunkSize){
	/*
	Draws a bitmap array stored in flash memory that is a user defined number of columns
	Can be both rotated and scaled
	The start at is because I wanted to create a massive sprite in a 1d array and this allowed me to load chuncks in different positions
	*/
	if (checkRotationValueAllowed(rotationValue)){
		byte yCounter = 0; //used for y position as if we use i it can have massive values
		for (uint16_t i = startAt; i < (startAt + chunkSize); i++){
			for (byte j = 0; j < ROWWIDTH; j++){
				if (pgm_read_byte(&array[i]) & (1 << j )){
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
}
;