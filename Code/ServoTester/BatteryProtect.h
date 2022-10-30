#pragma once
void SetupBatteryProtect();
void LoopBatteryProtect();
bool GetBatteryAlarmState();

// measured values with actual reistors result into 194 digits per Volt
// what I mesured:
// 823 at 4.2V
// 726 at 3.7V
// 649  at 3.3V
// Below 3.3V, display did not work sufficient anymore

#define DIGITS_PER_VOLT 194.0
