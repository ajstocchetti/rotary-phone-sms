#include <Arduino.h>
#include "smsRecipient.h"
#include "smsMessageContent.h"

void printSmsMessage() {
  Serial.print("SMS Message: ");
  Serial.println(getSmsMessage().c_str());
}

void printSmsRecipient() {
  Serial.print("Recipient phone number: ");
  Serial.println(getSmsNumber().c_str());
}

void printSmsInformation() {
  printSmsRecipient();
  printSmsMessage();
}
