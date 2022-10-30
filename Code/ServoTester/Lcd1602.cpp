#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "PinConfig.h"
#include "BatteryProtect.h"

#define CHARS_IN_LINE 16
#define LINES_ON_DISPLAY 2
#define DISPLAY_I2C_ADDRESS 0x27

LiquidCrystal_I2C lcd(DISPLAY_I2C_ADDRESS, CHARS_IN_LINE, LINES_ON_DISPLAY);

void LcdPrint(String text)
{	// no cursor setting here
	lcd.print(text);
}

void LcdPrint(int line, String text)
{
	lcd.setCursor(0, line);
	lcd.print(text);
}

void LcdLineClear(int line)
{
	lcd.setCursor(0, line);
	lcd.print("                "); // hardcoded 16 chars to avoid loop
}

void LcdClear()
{
	lcd.clear();
}

void Setup_Lcd1602()
{
	Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL); // SDA, SCL necessary for esp8266
	lcd.begin(CHARS_IN_LINE, LINES_ON_DISPLAY);
	lcd.noCursor();
	lcd.setBacklight(63);
	lcd.clear();
}

void LcdPrintPWM(int us, int row)
{
	String pwm;
	if (us < 1000)
		pwm = " ";
		
	pwm += String(us) + " us";

	lcd.setCursor(0, row);
	lcd.print(pwm);
}

void LcdHidePrintPWM(int row)
{
	lcd.setCursor(0, row);
	lcd.print("       ");
}

void LcdPrintVoltage(int measure)
{
	double Volt = measure / DIGITS_PER_VOLT;
	String voltage = String(Volt) + "V";
	lcd.setCursor(11, 1);
	lcd.print(voltage);
}

void LcdMarkActiveServo(int num)
{
	lcd.setCursor(8, num);
	lcd.print("*");
	lcd.setCursor(8, num == 0 ? 1 : 0);
	lcd.print(" ");
}