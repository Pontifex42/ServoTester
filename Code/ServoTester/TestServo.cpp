#include "TestServo.h"
#include "Lcd1602.h"

TestServo Servo1(PIN_SERVO1, 0);
TestServo Servo2(PIN_SERVO2, 1);

TestServo::TestServo(int pin, int lcdRow)
{
	myPin = pin;
	myLcdRow = lcdRow;
	myPWM = PWM_MIDDLE;
}

void TestServo::Setup()
{
	myServo.attach(myPin, PWM_MIN, PWM_MAX, myPWM);
	LcdPrintPWM();
}

void TestServo::ShutdownServo()
{
	myServo.detach();
	LcdHidePrintPWM();
}

void TestServo::RestartServo()
{
	myServo.attach(myPin, PWM_MIN, PWM_MAX, myPWM);
	LcdPrintPWM();
}

void TestServo::SetServo(int pwm)
{
	myServo.writeMicroseconds(pwm);
	myPWM = pwm;
	LcdPrintPWM();
}

void  TestServo::LcdPrintPWM()
{
	::LcdPrintPWM(myPWM, myLcdRow);
}

void TestServo::LcdHidePrintPWM()
{
	::LcdHidePrintPWM(myLcdRow);
}

void TestServo::IncrementPWM(int value)
{
	myPWM += value;
	if (myPWM > PWM_MAX)
		myPWM = PWM_MAX;
	else if (myPWM < PWM_MIN)
		myPWM = PWM_MIN;
	SetServo(myPWM);
}