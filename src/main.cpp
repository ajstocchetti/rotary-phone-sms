#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/* NEOPIXEL */
const int neoPixelPin = 25;
const int numNeoPixels = 7;
// #define PIXEL_TYPE WS2812B
#define PIXEL_TYPE NEO_GRBW + NEO_KHZ800
Adafruit_NeoPixel pixels(numNeoPixels, neoPixelPin, PIXEL_TYPE);
void setAllAsColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  uint32_t color = pixels.Color(r, g, b, w);
  for (uint8_t i = 0; i < numNeoPixels; i++) { pixels.setPixelColor(i, color); }
  pixels.show();
}
void turnOff() { setAllAsColor(0, 0, 0, 0); }
void turnRed() { setAllAsColor(255, 0, 0, 0); }
void turnGreen() { setAllAsColor(0, 255, 0, 0); }
void turnBlue() { setAllAsColor(0, 0, 255, 0); }
void turnWhite() { setAllAsColor(0, 0, 0, 255); }
/* end NEOPIXEL */

const int analogPin = 27;

const bool debugHardware = false;

const int highThreshold = 3800; //900; // anything over high is hung
const int lowThreshold = 3100; //550; // anything under low is open
const int delayTime = 20; // milliseconds;
const int steady_state_count = 6; // number count_since_change needs to pass to be considered in a static state
// static state is when the phone is not dialin
//   either off hung up or off the hook

enum lineState {
  on_hook,
  off_hook,
  changing,
};
lineState currentState = off_hook;
int readingsInCurrentState = steady_state_count; // start at steady state
int pulseCount = 0;

void alertHungUp() {
  Serial.println("Hung up");
  turnOff();
}
void alertLineOpen() {
  Serial.println("Line open");
  turnWhite();
}
void alertNumberDialed(const int num) {
  Serial.print("Dialed ");
  Serial.println(pulseCount);
  switch (num) {
    case 1:
      turnRed(); break;
    case 2:
      turnGreen(); break;
    case 3:
      turnBlue(); break;
    case 4:
      setAllAsColor(255, 128, 0, 0); break;
    case 5: setAllAsColor(0, 255, 128, 0); break;
    case 6: setAllAsColor(128, 0, 255, 0); break;
    case 7: setAllAsColor(128, 128, 0, 55); break;
    case 8: setAllAsColor(0, 128, 255, 76); break;
    case 9: setAllAsColor(34, 200, 98, 17); break;
  default:
    turnOff();
    break;
  }
}
void alertError() {
  Serial.println("ERROR");
}

int getLineAnalogValue() {
  return analogRead(analogPin); // read the input pin
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
        switch(currentState) {
          case on_hook: {
            alertHungUp();
            // if (pulseCount > 1) { /* do something - hung up in middle of dial */ }
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

  pixels.begin();
}

void loop()
{
  int lineReading = getLineAnalogValue();
  if (debugHardware == true) {
    Serial.println(lineReading);
  }
  processReading(lineReading);
  delay(delayTime);
}