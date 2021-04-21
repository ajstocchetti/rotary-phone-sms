#include <Arduino.h>
#include <CircularBuffer.h>

#include "neopixelhandler.h"
#include "operatormodes.h"
#include "smsManager.h"
#include "smsMessageContent.h"
#include "smsRecipient.h"

CircularBuffer<int, 5> inputSequence;

void broadcastPhoneInteraction(const int);
void alertHungUp() { broadcastPhoneInteraction(-10); }
void alertLineOpen() { broadcastPhoneInteraction(-15); }
void alertError() { broadcastPhoneInteraction(-100); }
void alertNumberDialed(const int num) { broadcastPhoneInteraction(num); }

void printOperatorStatuses();
void noop() { return; }

const int numOperatorModes = 6;
OperatorMode operatorFlags[numOperatorModes] = {
    OperatorMode("Serial", printOperatorStatuses, noop),
    OperatorMode("NeoPixel", noop, turnNeoPixelOff),
    OperatorMode("Set SMS Message", noop, noop),
    OperatorMode("Set SMS Recipient", noop, noop),
    OperatorMode("SMS Manager", noop, noop),
    OperatorMode("Operator Mode Display", printOperatorStatuses, printOperatorStatuses),
};
bool isSerialSet() { return operatorFlags[0].isActive(); }
bool isNeopixelSet() { return operatorFlags[1].isActive(); }
bool isSetMsgSet() { return operatorFlags[2].isActive(); }
void toggleMsgMode() {
  operatorFlags[2].toggle();
  if (operatorFlags[2].isActive()) {
    operatorFlags[3].disable();
    operatorFlags[4].disable();
  }
}
bool isSetRecipientSet() { return operatorFlags[3].isActive(); }
void toggleRecipientMode() {
  operatorFlags[3].toggle();
  if (operatorFlags[3].isActive()) {
    operatorFlags[2].disable();
    operatorFlags[4].disable();
  }
}
bool isSetManagerSet() { return operatorFlags[4].isActive(); }
void toggleManagerMode() {
  operatorFlags[4].toggle();
  if (operatorFlags[4].isActive()) {
    operatorFlags[2].disable();
    operatorFlags[3].disable();
  }
}

void initializeOperatorModes() {
  // start with Serial enabled
  operatorFlags[0].enable();
}

void printOperatorStatuses() {
  Serial.println("-------------------------------------");
  const int numOps = numOperatorModes;
  for (uint8_t x = 0; x < numOps; x++) {
    operatorFlags[x].printStatus();
  }
  Serial.println("-------------------------------------");
}

bool operatorModeCheck(const int input) {
  inputSequence.unshift(input);
  bool changedMode = false;

  // to set the an operator bit, need to:
  // - hang up
  // - pick up
  // - hang up
  // - pick up
  // - enter bit number
  if (!inputSequence.isFull()) return false;
  if (input < 1) return false;

  if (inputSequence[1] == -15 && inputSequence[2] == -10 &&
      inputSequence[3] == -15 && inputSequence[4] == -10) {
    // toggle flag
    uint8_t flagIndex = input - 1;
    switch (flagIndex) {
      case 0:
      case 1:
      case 5:
        operatorFlags[flagIndex].toggle();
        changedMode = true;
        break;
      case 2:
        toggleMsgMode();
        changedMode = true;
        break;
      case 3:
        toggleRecipientMode();
        changedMode = true;
        break;
      case 4:
        toggleManagerMode();
        changedMode = true;
        break;
      default:
        break;
    }
  }
  return changedMode;
}

void broadcastPhoneInteraction(const int phoneInput) {
  // First check if this is a mode change operation
  if (operatorModeCheck(phoneInput)) return;

  if (isSerialSet()) {
    switch (phoneInput) {
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

  if (isNeopixelSet()) notifyNeopixel(phoneInput);

  if (isSetMsgSet()) {
    if (notifySmsMessageText(phoneInput)) {
      printSmsMessage();
    }
  }

  if (isSetRecipientSet()) {
    if (notifySmsRecipientNumber(phoneInput)) {
      printSmsRecipient();
    }
  }

  if (isSetManagerSet()) {
    notifySmsManager(phoneInput);
  }
}