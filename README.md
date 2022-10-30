# ServoTester
The usual servo testing tools all suck, because there is no display to show the precise actual pulsewidth.
Furthermore, these tools only have a range from around 1000 to 2000 microseconds.
But many servos use a range from 500 up to 2500 microseconds and some can rotate more than 180 degrees.

So I designed a simple tool to check the range and precise setting of a servo.
Used hardware:
- my UniMiniESP8266 board, Schematics and Gerberfiles are in anaother github repository of mine
  https://github.com/Pontifex42/UniMiniESP8266
  Any other microcontroller will do as well, but code has to be modified a bit.
- A 18650 cell with battery holder.
- A HW-373 breakout board battery charge controller (TP4056), optional
- A usual 1602 LCD display
- An rotary encoder HWbreakout board -40
- a small switch
- a piece of perfboard tosolder dupont connector
- a self printed case
- some Female-Female Dupont wires
- some hot glue
- M 2,5 mm melt-in thread inserts and M2,5 screws

The wiring is obvious if you read "PinConfig.h", so there is no schematics here.

Questions? Try "github@hartwig.org" and I will answer quick.
