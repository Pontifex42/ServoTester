#pragma once

void SetupRotary();
void LoopRotary();
bool WasRotaryButtonPressed(bool reset = true);
int GetRotaryIncrement(bool reset = true);
void FlushRotaryEvents();
bool IsButtonPressed();