#include "dial_to_text_class.h"
#include "smsManager.h"

DialToText smsMessage;

bool notifySmsMessageText(const int input) {
  bool changed = smsMessage.onDialInput(input);
  if (changed) {
    printSmsMessage();
  }
  return changed;
}

String getSmsMessage() { return smsMessage.content(); }
