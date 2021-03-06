#include "operator_modes_class.h"

void noop() { return; }

bool OperatorMode::isActive() { return isActiveB; }
void OperatorMode::toggle() {
  if (isActive()) {
    isActiveB = false;
    onDisable();
  } else {
    isActiveB = true;
    onEnable();
  }
  printStatus();
}
void OperatorMode::disable() {
  if (!isActiveB) return;
  else toggle();
}
void OperatorMode::enable() {
  if (isActiveB) return;
  else toggle();
}
void OperatorMode::printStatus() {
  Serial.print("Mode ");
  Serial.print(modeName);
  Serial.print(" is ");
  Serial.println(isActive() ? "enabled" : "disabled");
}