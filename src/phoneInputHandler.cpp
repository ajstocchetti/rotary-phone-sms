#include <Arduino.h>
#include "neopixelhandler.h"
#include <CircularBuffer.h>


CircularBuffer<int, 5> inputSequence;


bool operatorFlags[5] = {true, true, false, false, false};
bool isSerialSet() { return operatorFlags[0]; }
bool isNeopixelSet() { return operatorFlags[1]; }


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
        operatorFlags[flagIndex] = !operatorFlags[flagIndex];

        for (uint8_t x = 0; x < 5; x++) {
          Serial.print("Flag "); Serial.print(x); Serial.print(" is "); Serial.println(operatorFlags[x]);
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
