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

Copy the .h and .cpp files from max7219MatrixLib/src/ to your project directory and #include them to use the library

The usual drawPixel, plotLine, plotSquare, plotCircle options are here.

However, I've made it so that you can easily displays your own byte based (e.g, B10000001) bitmap arrays for ease of displaying simple pixel art.
There is an option for 8bit, 16bit (these refer to the amount of columns and still require a byte array) or a custom column amount.

These can be easily combined to create much larger and impressive sprites

It's good practise to wipe the screen buffer, draw what you need to in the screen buffer, then send the screen buffer to the displays.

Characters are an UTF-8 8x8 font and characters includes U+0020 to U+007E. Just use a single letter in 's using the draw letter functions.

Usage Examples
--------------

[Characters being displayed and rotated](https://youtu.be/Md17N4Kh_fQ)

[Full demo as of 2022-12-12](https://www.youtube.com/watch?v=S0076WmlEyU)

[MPU6050 controlled LED Sand effect](https://youtu.be/VmF_7_NJ_H0)

Bitmaps image

![Sprites image](https://i.ibb.co/1s3WGxz/20221112-190850.jpg)
