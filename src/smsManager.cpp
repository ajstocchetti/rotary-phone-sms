#include <Arduino.h>
#include "smsRecipient.h"
#include "smsMessageContent.h"

void printSmsMessage() {
  Serial.println("SMS Message:");
  Serial.print("\t");
  Serial.println(getSmsMessage().c_str());
}

void printSmsRecipient() {
  Serial.print("SMS Recipient: ");
  Serial.println(getSmsNumber().c_str());
  Serial.println("");
}

void printSmsInformation() {
  Serial.println("SMS Information:");
  printSmsRecipient();
  printSmsMessage();
}
