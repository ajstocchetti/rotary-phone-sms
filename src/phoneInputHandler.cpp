#include <Arduino.h>
#include "neopixelhandler.h"
// #include <boost/circular_buffer.hpp>

uint operatorModeMask = 0b000011;
/*
  bitmask parts
  1: log serial
  2: neopixel
*/


/*
  hang up: -10
  line open: -15
  error: -100
*/

void broadcastMessage(const int phoneInput)
{
  if ((operatorModeMask && 0b1) == 0b1) {
    switch (phoneInput)
    {
      case -100:
        Serial.println("ERROR"); break;
      case -15:
        Serial.println("Line open"); break;
      case -10:
        Serial.println("Hung up"); break;
      default:
        // TODO: error check for negative numbers?
        Serial.print("Dialed ");
        Serial.println(phoneInput);
        break;
    }
  }
  if ((operatorModeMask & 0b10) == 0b10) notifyNeopixel(phoneInput);
}

void alertHungUp()
{
  broadcastMessage(-10);
}

void alertLineOpen()
{
  broadcastMessage(-15);
}

void alertError()
{
  broadcastMessage(-100);
}

void alertNumberDialed(const int num)
{
  broadcastMessage(num);
}