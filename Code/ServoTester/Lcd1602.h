#pragma once
void Setup_Lcd1602();
void LcdClear();
void LcdLineClear(int line);
void LcdPrint(int line, String text);
void LcdPrint(String text);

void LcdPrintPWM(int us, int row);
void LcdHidePrintPWM(int row);
void LcdPrintVoltage(int measure);
void LcdMarkActiveServo(int num);
