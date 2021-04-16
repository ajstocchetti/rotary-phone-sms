#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

const int neoPixelPin = 25;
const int numNeoPixels = 7;

// #define PIXEL_TYPE WS2812B
#define PIXEL_TYPE NEO_GRBW + NEO_KHZ800
Adafruit_NeoPixel pixels(numNeoPixels, neoPixelPin, PIXEL_TYPE);

void initiateNeopixel()
{
  pixels.begin();
}

void setAllAsColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  uint32_t color = pixels.Color(r, g, b, w);
  for (uint8_t i = 0; i < numNeoPixels; i++)
  {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}
void turnOff() { setAllAsColor(0, 0, 0, 0); }
void turnRed() { setAllAsColor(255, 0, 0, 0); }
void turnGreen() { setAllAsColor(0, 255, 0, 0); }
void turnBlue() { setAllAsColor(0, 0, 255, 0); }
void turnWhite() { setAllAsColor(0, 0, 0, 255); }


void notifyNeopixel(const int num)
{
  switch (num)
  {
    case -10:
      turnOff();
      break;
    case -15:
      turnWhite();
      break;
    case 1:
      turnRed();
      break;
    case 2:
      turnGreen();
      break;
    case 3:
      turnBlue();
      break;
    case 4:
      setAllAsColor(255, 128, 0, 0);
      break;
    case 5:
      setAllAsColor(0, 255, 128, 0);
      break;
    case 6:
      setAllAsColor(128, 0, 255, 0);
      break;
    case 7:
      setAllAsColor(128, 128, 0, 55);
      break;
    case 8:
      setAllAsColor(0, 128, 255, 76);
      break;
    case 9:
      setAllAsColor(34, 200, 98, 17);
      break;
    default:
      turnOff();
      break;
  }
}
