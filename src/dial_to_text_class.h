#include <Arduino.h>
#include <CircularBuffer.h>

class DialToText {
  private:
    String text;
    CircularBuffer<int, 2> dialSequence;
    bool lastInputChangedText;
    String addToMessage(char c);
    bool deleteChar();
    bool isValidInputSequence(int, int);
  
  public:
    DialToText(String startText = "") {
      text = startText;
      lastInputChangedText = false;
    };
    bool onDialInput(const int);
    String content();
    void clear();
};
