# max7219MatrixLib
Library for the MAX 7219 LED Matrixes

What is the purpose of this library
===================================

There are many different libraries avaiable for the Max 7219 series of LED Matrixes.

However, I wanted to create one with the following objectives :

- Light LEDs with x, y co-ordinates
- Keep memory usage to a minimum
- Make updating the matrixes as fast as I could
- Adaptable to different matrix configs

Usage
=====

Wiring
------

This library was developed on an Arduino Uno and uses the following pins for SPI :

10 - CS
11 - MOSI
13 - CLK

Matrix orientation
------------------
I developed using both an 32x8 and 3 32x8 matrixes daisy chained together

The right hand side of the matrix is connected to the Arduino and left hand side is used for connecting other matrixes

Co-ordinates are top left corner is 0, and y increases as you move down the matrix similar to TFT screens.

0 X - >

Y

|

V

Constructor
-----------

LedMatrix(uint8_t noOfModulesWide, uint8_t noOfModulesHigh)

A single matrix would be 1 module wide and 1 module wide

4 Matrixes in a 2x2 formation would be 2 modules wide and 2 modules high

_Connecting 2 32x8 matrixes in a 32x16 formation would be 4 modules wide and 2 modules high_

__Note, each row must have the same number of matrixes. It's not possible with this library to have 1 row with 3 matrixes, then a 2nd row with 4 matrixes__

Usage
-----
The usual drawPixel, plotLine, plotSquare, plotCircle options are here.

However, I've made it so that you can easily displays your own byte based (e.g, B10000001) bitmap arrays for ease of displaying simple pixel art.
There is an option for 8bit, 16bit (these refer to the amount of columns and still require a bit array) or a custom column amount

