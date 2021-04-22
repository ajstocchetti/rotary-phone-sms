#include <Arduino.h>

void noop();

class OperatorMode {
 private:
  bool isActiveB;
  String modeName;
  void (*onEnable)();
  void (*onDisable)();

 public:
  OperatorMode(String name, bool startEnabled = false, void (*onEn)() = noop, void (*onDis)() = noop) {
    modeName = name;
    isActiveB = startEnabled;
    onEnable = onEn;
    onDisable = onDis;
  }
  void printStatus();
  bool isActive();
  void enable();
  void disable();
  void toggle();
};