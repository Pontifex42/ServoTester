#pragma once
#include <Servo.h>
#include "PinConfig.h"

// Servo timing is usually in a range of 1000us up to 2000us, but some servos act on a range 500us-2500us and some do more
#define PWM_MIN	100 // ensure we support more than the full range
#define PWM_MIDDLE 1500
#define PWM_MAX 3000 // ensure we support more than the full range

class TestServo
{
public:
	TestServo(int pin, int lcdColumn);
	void Setup();
	void ShutdownServo();
	void RestartServo();
	void SetServo(int pwm);
	void LcdPrintPWM();
	void LcdHidePrintPWM();
	void IncrementPWM(int value);
private:
	Servo myServo;
	int myPin;
	int myLcdRow;
	int myPWM;
};

extern TestServo Servo1;
extern TestServo Servo2;
