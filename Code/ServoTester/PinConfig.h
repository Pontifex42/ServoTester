#pragma once
#include "Arduino.h"

#define PIN_M1 0	// Used for bootloader and Incremental-Switch, too. Is pulled up externally.
#define PIN_M2 2	// This will be our only servo output. Is pulled up externally.
#define PIN_M3 4 // Used for I2C
#define PIN_M4 5 // Used for I2C
#define PIN_M5 12
#define PIN_M6 13 // Re-used for PWM bin
#define PIN_MOT_A	14
#define PIN_MOT_B	15
#define PIN_SPARE	16

// Overdischarge protection
#define PIN_CHECK_BATT	A0

// Rotary encoder
#define PIN_ROT_A	PIN_M5
#define PIN_ROT_B	PIN_M6
#define PIN_ROT_S	PIN_M1 // Used to activate bootloader, too.  Is pulled up externally.

// Display 1302
#define PIN_I2C_SDA	PIN_M3
#define PIN_I2C_SCL	PIN_M4

// actual used servo
#define PIN_SERVO1 PIN_M2
#define PIN_SERVO2 PIN_SPARE

#define DEBUG_PRINTLN(a) Serial.println(a)
//#define DEBUG_PRINTLN(a)
#define DEBUG_PRINT(a) Serial.print(a)
//#define DEBUG_PRINT(a)
