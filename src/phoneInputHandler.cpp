#include <Arduino.h>
#include <CircularBuffer.h>

#include "neopixelhandler.h"
#include "operatormode.h"
#include "smsManager.h"
#include "smsMessageContent.h"
#include "smsRecipient.h"

CircularBuffer<int, 5> inputSequence;

void printOperatorStatuses();
void enableSmsMsgMode();
void enableSmsRcptMode();
void enableSmsManagerMode();
void noop() { return; }
OperatorMode operatorFlags[5] = {
    OperatorMode("Serial", printOperatorStatuses, printOperatorStatuses),
    OperatorMode("NeoPixel", noop, turnNeoPixelOff),
    OperatorMode("Set SMS Message", enableSmsMsgMode, noop),
    OperatorMode("Set SMS Recipient", enableSmsRcptMode, noop),
    OperatorMode("SMS Manager", enableSmsMsgMode, noop),
};

void initializeOperatorModes() {
  // start with Serial enabled
  if (!operatorFlags[0].isActive()) {
    operatorFlags[0].toggle();
  }
}

bool isSerialSet() { return operatorFlags[0].isActive(); }
bool isNeopixelSet() { return operatorFlags[1].isActive(); }
bool isSetMsgSet() { return operatorFlags[2].isActive(); }
bool isSetRecipientSet() { return operatorFlags[3].isActive(); }

void printOperatorStatuses() {
  int numOps = sizeof(operatorFlags) / sizeof(operatorFlags[0]);
  for (uint8_t x = 0; x < numOps; x++) {
    operatorFlags[x].printStatus();
  }
}

void enableSmsMsgMode() {
  operatorFlags[3].disable();
  operatorFlags[4].disable();
}
void enableSmsRcptMode() {
  operatorFlags[2].disable();
  operatorFlags[4].disable();
}
void enableSmsManagerMode() {
  operatorFlags[2].disable();
  operatorFlags[3].disable();
  printSmsInformation();
}

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

  if (inputSequence[1] == -15 && inputSequence[2] == -10 &&
      inputSequence[3] == -15 && inputSequence[4] == -10) {
    // toggle flag
    uint8_t flagIndex = input - 1;
    operatorFlags[flagIndex].toggle();
  }
}

void broadcastPhoneInteraction(const int phoneInput) {
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

  // handle operator mode checks _after_ alerting others
  checkOperatorBits(phoneInput);
}

void alertHungUp() { broadcastPhoneInteraction(-10); }

void alertLineOpen() { broadcastPhoneInteraction(-15); }

void alertError() { broadcastPhoneInteraction(-100); }

void alertNumberDialed(const int num) { broadcastPhoneInteraction(num); }
