#include "operatormodes.h"

bool OperatorMode::isActive() { return isActiveB; }
void OperatorMode::toggle() {
  if (isActive()) {
    isActiveB = false;
    onDisable();
  } else {
    isActiveB = true;
    onEnable();
  }
}
void OperatorMode::disable() {
  if (!isActiveB) return;
  else onDisable();
}
void OperatorMode::enable() {
  if (isActiveB) return;
  else onEnable();
}
void OperatorMode::printStatus() {
  Serial.print("Mode ");
  Serial.print(modeName);
  Serial.print(" is ");
  Serial.println(isActive() ? "enabled" : "disabled");
}