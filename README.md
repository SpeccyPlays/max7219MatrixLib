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



