#include <Arduino.h>
#include "neopixelhandler.h"
#include <CircularBuffer.h>


CircularBuffer<int, 5> inputSequence;

void noop() { return; }
class OperatorMode {
  private:
    bool isActiveB;
    String modeName;
    void (*onEnable)();
    void (*onDisable)();
  public:
    OperatorMode(String act, bool enab = false, void (*onEn)() = noop, void (*onDis)() = noop) {
      modeName = act;
      isActiveB = enab;
      onEnable = onEn;
      onDisable = onDis;
    }
    void printStatus();
    bool isActive();
    void toggle();
};

bool OperatorMode::isActive() { return isActiveB; }
void OperatorMode::toggle() {
  isActiveB = !isActiveB;
  if (isActiveB) onEnable();
  else onDisable();
}
void OperatorMode::printStatus() {
  Serial.print("Mode ");
  Serial.print(modeName);
  Serial.print(" is ");
  Serial.println(isActive() ? "enabled" : "disabled");
}

OperatorMode serialMode = OperatorMode("Serial", true);
OperatorMode neopixelMode = OperatorMode("NeoPixel", true, noop, turnNeoPixelOff);
OperatorMode placeholderMode = OperatorMode("Placeholder", true);

OperatorMode operatorFlags[5] = {
    OperatorMode("Serial", true),
    OperatorMode("NeoPixel", true, noop, turnNeoPixelOff),
    OperatorMode("Placeholder"),
    OperatorMode("Placeholder"),
    OperatorMode("Placeholder"),
};
bool isSerialSet() { return operatorFlags[0].isActive(); }
bool isNeopixelSet() { return operatorFlags[1].isActive(); }


void checkOperatorBits(const int input) {
  inputSequence.unshift(input);

  // to set the an operator bit, need to:
  // - hang up
  // - pick up
  // - hang up
  // - pick up
  // - enter bit number
  if (!inputSequence.isFull()) return;
  if (input < 1) return;

  if (inputSequence[1] == -15
      && inputSequence[2] == -10
      && inputSequence[3] == -15
      && inputSequence[4] == -10) {
        // toggle flag
        uint8_t flagIndex = input - 1;
        operatorFlags[flagIndex].toggle();

        for (uint8_t x = 0; x < 5; x++) {
          operatorFlags[x].printStatus();
        }
  }
}

void broadcastPhoneInteraction(const int phoneInput)
{
  if (isSerialSet())
  {
    switch (phoneInput)
    {
    case -100:
      Serial.println("ERROR");
      break;
    case -15:
      Serial.println("Line open");
      break;
    case -10:
      Serial.println("Hung up");
      break;
    default:
      // TODO: error check for negative numbers?
      Serial.print("Dialed ");
      Serial.println(phoneInput);
      break;
    }
  }
  if (isNeopixelSet())
    notifyNeopixel(phoneInput);

  // handle operator mode checks _after_ alerting others
  checkOperatorBits(phoneInput);
}

void alertHungUp()
{
  broadcastPhoneInteraction(-10);
}

void alertLineOpen()
{
  broadcastPhoneInteraction(-15);
}

void alertError()
{
  broadcastPhoneInteraction(-100);
}

void alertNumberDialed(const int num)
{
  broadcastPhoneInteraction(num);
}
