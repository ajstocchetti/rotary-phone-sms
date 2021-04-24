#include "dial_to_text_class.h"
#include "smsManager.h"

DialToText smsMessage;

bool notifySmsMessageText(const int input, bool logContent = true) {
  bool changed = smsMessage.onDialInput(input);
  if (changed && logContent) {
    printSmsMessage();
  }
  return changed;
}

String getSmsMessage() { return smsMessage.content(); }
