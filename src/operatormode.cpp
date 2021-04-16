#include "operatormode.h"

bool OperatorMode::isActive() { return isActiveB; }
void OperatorMode::toggle() {
  isActiveB = !isActiveB;
  if (isActiveB)
    onEnable();
  else
    onDisable();
}
void OperatorMode::disable() {
  if (!isActiveB) return;
  else disable();
}
void OperatorMode::printStatus() {
  Serial.print("Mode ");
  Serial.print(modeName);
  Serial.print(" is ");
  Serial.println(isActive() ? "enabled" : "disabled");
}