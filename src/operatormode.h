#include <Arduino.h>

/*
  want to supply OperatorMode constructor with default noop paramters for
  onEnable and onDisable, but getting weird linker errors... So no default params
  for now
  void noop() { return; }
  OperatorMode(String name, void (*onEn)() = noop, void (*onDis)() = noop) {
*/

class OperatorMode {
 private:
  bool isActiveB;
  String modeName;
  void (*onEnable)();
  void (*onDisable)();

 public:
  OperatorMode(String name, void (*onEn)(), void (*onDis)()) {
    modeName = name;
    isActiveB = false;
    onEnable = onEn;
    onDisable = onDis;
  }
  void printStatus();
  bool isActive();
  void toggle();
  void disable();
};