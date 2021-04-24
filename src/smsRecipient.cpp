#include <Arduino.h>
#include "smsManager.h"

String recipientNumber = "";
char intToChar(int digit);
bool checkForEmptyCode();

bool notifySmsRecipientNumber(const int input, bool logContent = true) {
  if (input > 0 && input < 11) {
    recipientNumber += (intToChar(input));
    checkForEmptyCode();
    if (logContent) { printSmsRecipient(); }
    return true;
  }
  return false;
}

bool checkForEmptyCode() {
  // dial 555 to clear the phone number
  int numDigits = recipientNumber.length();
  if (numDigits > 2) {
    if (recipientNumber[numDigits - 1] == '5' &&
        recipientNumber[numDigits - 2] == '5' &&
        recipientNumber[numDigits - 3] == '5') {
      recipientNumber.clear();
      return true;
    }
  }

  return false;
}

String getSmsNumber() { return recipientNumber; }

char intToChar(int digit) {
  // digit must be validated already
  switch (digit) {
    case 10:
      return '0';
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
  }
  throw "Invalid input";
}