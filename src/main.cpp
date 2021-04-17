#include <Arduino.h>

#include "neopixelhandler.h"
#include "phoneInputHandler.h"

const int analogPin = 34;
const bool debugHardware = false;

const int highThreshold = 3800;    // 900; // anything over high is hung
const int lowThreshold = 1100;     // 550; // anything under low is open
const int delayTime = 20;          // milliseconds;
const int steady_state_count = 6;  // number count_since_change needs to pass to
                                   // be considered in a static state
// static state is when the phone is not dialin
//   either off hung up or off the hook

enum lineState {
  on_hook,
  off_hook,
  changing,
};
lineState currentState = off_hook;
int readingsInCurrentState = steady_state_count;  // start at steady state
int pulseCount = 0;

int getLineAnalogValue() {
  return analogRead(analogPin);  // read the input pin
}

lineState getStateType(const int analogVal) {
  if (analogVal > highThreshold) return on_hook;
  if (analogVal < lowThreshold) return off_hook;

  // Serial.print("Error reading: "); Serial.println(analogVal);
  return changing;
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
            // if (pulseCount > 1) { /* do something - hung up in middle of dial
            // */ }
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
          case changing: {
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
  initializeOperatorModes();
  initiateNeopixel();
}

void loop() {
  int lineReading = getLineAnalogValue();
  if (debugHardware == true) Serial.println(lineReading);
  processReading(lineReading);
  delay(delayTime);
}