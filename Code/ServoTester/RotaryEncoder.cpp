#include "PinConfig.h"
#include "RotaryEncoder.h"
#include "Arduino.h"

// #define DEBUG_ROTARYENCODER

#ifndef DEBUG_ROTARYENCODER
#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#define DEBUG_PRINTLN(a)
#undef DEBUG_PRINT
#define DEBUG_PRINT(a)
#endif
#endif

#define DEBOUNCE_TIME 100

#define IL	42	// illegal
#define CW	1	// Clockwise
#define CCW	-1	// Counter clockwise
#define NO	0	// No Move
//   old	 new
//	A	B	A	B	change
//	0	0	0	0	NO
//	0	0	0	1	CW
//	0	0	1	0	CCW
//	0	0	1	1	IL
//	0	1	0	0	CCW
//	0	1	0	1	NO
//	0	1	1	0	IL
//	0	1	1	1	CW
//	1	0	0	0	CW
//	1	0	0	1	IL
//	1	0	1	0	NO
//	1	0	1	1	CCW
//	1	1	0	0	IL
//	1	1	0	1	CCW
//	1	1	1	0	CW
//	1	1	1	1	NO
const int matrix[2][2][2][2] = { NO, CW, CCW, IL,
							CCW, NO, IL, CW,
							CW, IL, NO, CCW,
							IL, CCW, CW, NO };


long posCnt = 0;
int lastA;
int lastB;
int lastButton;         
int newA;
int newB;
int newButton;
bool buttonpress = false;

void OnRotary()
{
	int inc = matrix[lastA][lastB][newA][newB];
	lastA = newA;
	lastB = newB;
	if (inc != IL)
		posCnt += inc;
	DEBUG_PRINTLN("Rotary increment " + String(inc));
}

void OnRotaryButton()
{
	if(newButton != lastButton)
	{
		lastButton = newButton;
		DEBUG_PRINTLN("Button state " + String(newButton));
		if (newButton == LOW) // level is low on button pressed
			buttonpress = true;
	}
}


bool WasRotaryButtonPressed(bool reset)
{
	bool ret;
	ret = buttonpress;
	if (reset)
		buttonpress = false;
	
	return ret;
}

bool IsButtonPressed()
{
	return !digitalRead(PIN_ROT_S);
}

#define INC_PER_STEP 4 // some encoders have 4 increments per step
int GetRotaryIncrement(bool reset)
{
	int diff = posCnt / INC_PER_STEP;
	if (diff && reset)
		posCnt %= INC_PER_STEP;

	return diff;
}

void FlushRotaryEvents()
{
	GetRotaryIncrement(true);
	WasRotaryButtonPressed(true);
}

void IRAM_ATTR ISR_OnRotaryA()
{
	newA = digitalRead(PIN_ROT_A);
	int inc = matrix[lastA][lastB][newA][newB];
	lastA = newA;
	if (inc != IL)
		posCnt += inc;
}

void IRAM_ATTR ISR_OnRotaryB()
{
	newB = digitalRead(PIN_ROT_B);
	int inc = matrix[lastA][lastB][newA][newB];
	lastB = newB;
	if (inc != IL)
		posCnt += inc;
}


void SetupRotary()
{
	pinMode(PIN_ROT_A, INPUT_PULLUP);
	pinMode(PIN_ROT_B, INPUT_PULLUP);
	pinMode(PIN_ROT_S, INPUT_PULLUP);

	newA = lastA = digitalRead(PIN_ROT_A);
	newB = lastB = digitalRead(PIN_ROT_B);
	newButton = lastButton = digitalRead(PIN_ROT_S);
	posCnt = 0;

	attachInterrupt(digitalPinToInterrupt(PIN_ROT_A), ISR_OnRotaryA, CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_ROT_B), ISR_OnRotaryB, CHANGE);
	DEBUG_PRINTLN("Rotary setup done");
}

void LoopRotary()
{
	static bool butChangePending = false;
	static unsigned long timerButton;
	int tmpButton = digitalRead(PIN_ROT_S);
	if (tmpButton != newButton)
	{	// button state changed since last read, start timing to handle bouncing
		timerButton = micros() + DEBOUNCE_TIME;
		newButton = tmpButton;
		butChangePending = true;
		DEBUG_PRINTLN("Button changed to " + String(newButton));
		return;
	}

	if (butChangePending && (micros() >= timerButton))
	{
		butChangePending = false;
		DEBUG_PRINTLN("Button debounced");
		OnRotaryButton();
	}
}
