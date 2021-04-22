#include <Arduino.h>

#include "neopixelhandler.h"
#include "phoneInputHandler.h"

const int analogPin = 34;
const bool debugHardware = false;

const int highThreshold = 3800;  // anything over high is hung
const int lowThreshold = 1100;   // anything under low is open
const int delayTime = 20;        // milliseconds;
const int steady_state_count = 6;
/*
  steady_state_count: the number of line readings/intervals that need to pass
  with the line reading being constant for the phone to be considered
  at steady state (not dialing)
*/

enum lineState {
  on_hook,
  off_hook,
  bad_read,
};
lineState currentState;
int readingsInCurrentState = steady_state_count;  // start at steady state
int pulseCount = 0;

int getLineAnalogValue() {
  return analogRead(analogPin);  // read the input pin
}

lineState getStateType(const int analogVal) {
  if (analogVal > highThreshold) return on_hook;
  if (analogVal < lowThreshold) return off_hook;

  // if read is between high and low threshold, it is invalid
  return bad_read;
}

void processReading(int latestReading) {
  lineState lastState = currentState;
  currentState = getStateType(latestReading);

  if (currentState != lastState) {
    // state has changed
    readingsInCurrentState = 0;
    if (currentState == on_hook) {
      pulseCount++;
    }
  } else {
    // advance our count of how long we've been in this state
    // but do not cross steady_state_count so that our ints don't roll over
    if (readingsInCurrentState < steady_state_count) {
      readingsInCurrentState++;
      // now that we've advanced our state count, are we in steady state?
      if (readingsInCurrentState == steady_state_count) {
        // we just reached steady state
        switch (currentState) {
          case on_hook: {
            alertHungUp();
            // TODO: consider throwing a warning if pulseCount > 1 -> hung up in the middle of dialing
            pulseCount = 0;
            break;
          }
          case off_hook: {
            if (pulseCount == 0) {
              alertLineOpen();
            } else {
              alertNumberDialed(pulseCount);
            }
            pulseCount = 0;
            break;
          }
          case bad_read: {
            alertError();
            // pulseCount = 0;
            break;
          }
        }
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  currentState = on_hook;
  initiateNeopixel();
}

void loop() {
  int lineReading = getLineAnalogValue();
  if (debugHardware == true) Serial.println(lineReading);
  processReading(lineReading);
  delay(delayTime);
}