#include "Arduino.h"
#include "BatteryProtect.h"
#include "PinConfig.h"
#include "Lcd1602.h"

//#define DEBUG_BATTERYPROTECT

#ifndef DEBUG_BATTERYPROTECT

#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#define DEBUG_PRINTLN(a)
#undef DEBUG_PRINT
#define DEBUG_PRINT(a)
#endif

#endif

#define BATTERY_CHECK_INTERVALL 500000  // 2 times a second will do nicely
#define THRESHOLD_VALUE (3.3*DIGITS_PER_VOLT)
#define HYSTERESIS_VALUE (3.5*DIGITS_PER_VOLT)

bool BatteryAlarmState = false;
int batteryLowCounter = 0;
uint16_t measuredBattery = THRESHOLD_VALUE;

unsigned long nextTimeToCheckBattery;

bool GetBatteryAlarmState()
{
	return BatteryAlarmState;
}

void SetBatteryAlarmState(bool state)
{
	BatteryAlarmState = state;
}

void LoopBatteryProtect()
{
	unsigned long now = micros();
	if (now < nextTimeToCheckBattery)
		return;
		
	nextTimeToCheckBattery = now + BATTERY_CHECK_INTERVALL;

	measuredBattery = analogRead(PIN_CHECK_BATT);
	LcdPrintVoltage(measuredBattery);

	DEBUG_PRINT(measuredBattery);
	DEBUG_PRINT("   ");
	if (GetBatteryAlarmState())
	{	// needs to be a certain time above a higher voltage to return to non-alarm-state
		if (measuredBattery > HYSTERESIS_VALUE)
			batteryLowCounter--;
		else
			batteryLowCounter = 10;

		if (batteryLowCounter <= 0)
		{
			batteryLowCounter = 0;
			SetBatteryAlarmState(false);
		}
	}
	else
	{
		if (measuredBattery < THRESHOLD_VALUE)
			batteryLowCounter++;
		else
			batteryLowCounter = 0;

		if (batteryLowCounter >= 10) // one second with undervoltage
		{
			batteryLowCounter = 10;
			SetBatteryAlarmState(true);
		}
	}
	DEBUG_PRINTLN(batteryLowCounter);
}

void SetupBatteryProtect()
{
	pinMode(PIN_CHECK_BATT, INPUT);
	nextTimeToCheckBattery = micros() + BATTERY_CHECK_INTERVALL;
}
