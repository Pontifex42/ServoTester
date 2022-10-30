/*
 Name:		ServoTester.ino
 Created:	22.10.2022 16:23:32
 Author:	Pontifex Maximus
*/
#include "PinConfig.h"
#include "Lcd1602.h"
#include "BatteryProtect.h"
#include "RotaryEncoder.h"
#include "TestServo.h"
#include "Lcd1602.h"

bool HandleBatteryAlarm()
{
	if (!GetBatteryAlarmState())
		return false;

	static bool AlarmWasActive = false;

	bool AlarmIsActive = GetBatteryAlarmState();
	if (AlarmWasActive && !AlarmIsActive)
	{	// return to normal state
		AlarmWasActive = false;
		DEBUG_PRINTLN("Battery alarm ends.");

		Servo1.RestartServo();
		Servo2.RestartServo();
	}
	else if (!AlarmWasActive && AlarmIsActive)
	{	// Go to power save mode.
		AlarmWasActive = true;
		DEBUG_PRINTLN("Battery alarm!");

		Servo1.ShutdownServo();
		Servo2.ShutdownServo();
	}

	return AlarmIsActive;
}

TestServo* currentServo;
void HandleBlinkState(bool isButtonPressed)
{
	static bool DisplayIsOn1 = true;
	static bool DisplayIsOn2 = true;
	static unsigned long nextBlinkTime;

	if (!isButtonPressed)
	{
		if (!DisplayIsOn1)
		{
			DisplayIsOn1 = true;
			Servo1.LcdPrintPWM();
		}
		if (!DisplayIsOn2)
		{
			DisplayIsOn2 = true;
			Servo2.LcdPrintPWM();
		}
		return;
	}

#define BLINK_INTERVAL 400000
	unsigned long now = micros();
	if (now > nextBlinkTime)
	{
		nextBlinkTime = now + BLINK_INTERVAL;
		if (currentServo == &Servo1)
		{
			if (DisplayIsOn1)
			{
				DisplayIsOn1 = false;
				Servo1.LcdHidePrintPWM();
			}
			else
			{
				DisplayIsOn1 = true;
				Servo1.LcdPrintPWM();
			}

			if (!DisplayIsOn2)
			{
				DisplayIsOn2 = true;
				Servo2.LcdPrintPWM();
			}
		}
		else
		{
			if (DisplayIsOn2)
			{
				DisplayIsOn2 = false;
				Servo2.LcdHidePrintPWM();
			}
			else
			{
				DisplayIsOn2 = true;
				Servo2.LcdPrintPWM();
			}

			if (!DisplayIsOn1)
			{
				DisplayIsOn1 = true;
				Servo1.LcdPrintPWM();
			}
		}
	}
}

int currentIncrement;
#define PWM_INCREMENT_SMALL 10
#define PWM_INCREMENT_BIG 50
void setup() 
{
    Serial.begin(115200);

    SetupRotary();
    Setup_Lcd1602();
    SetupBatteryProtect();
	Servo1.Setup();
	Servo2.Setup();
	currentServo = &Servo1;
	LcdMarkActiveServo(0);
	currentIncrement = PWM_INCREMENT_BIG;
	DEBUG_PRINTLN("Setup done");
}

void loop() 
{
    LoopBatteryProtect();
	HandleBatteryAlarm();

	LoopRotary();

	bool buttonPressed = WasRotaryButtonPressed();
	if (buttonPressed)
	{
		if (currentIncrement == PWM_INCREMENT_BIG)
			currentIncrement = PWM_INCREMENT_SMALL;
		else
			currentIncrement = PWM_INCREMENT_BIG;
	}

	bool isButtonPressed = IsButtonPressed();

	HandleBlinkState(isButtonPressed);

    int rotated = GetRotaryIncrement();
	if (!rotated)
		return;

	if (isButtonPressed)
	{
		// does not matter in which direction rotated. Just switch to the other servo.
		if (currentServo == &Servo1)
		{
			currentServo = &Servo2;
			LcdMarkActiveServo(1);
		}
		else
		{
			currentServo = &Servo1;
			LcdMarkActiveServo(0);
		}
	}
	else
	{
		if (rotated < 0)
			currentServo->IncrementPWM(currentIncrement);
		else if (rotated > 0)
			currentServo->IncrementPWM(-currentIncrement);
	}
}
