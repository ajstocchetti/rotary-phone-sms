#include "dial_to_text_class.h"

DialToText smsMessage;

bool notifySmsMessageText(const int input) {
  return smsMessage.onDialInput(input);
}

String getSmsMessage() { return smsMessage.content(); }
