#include <string>
#include <CircularBuffer.h>

std::string theMessage = "";
CircularBuffer<int, 2> dialSequence;
bool lastDialAddedChar = false;

bool didInputAlterMsg(int charGroup, int charPosition);

bool notifySmsMessageText(const int input)
{
  dialSequence.push(input);
  if (lastDialAddedChar || !dialSequence.isFull()) {
    // if the last action added a char to the message
    // then this one shouldn't
    // So don't try to add,
    // but flip the tracker
    lastDialAddedChar = false;
  } else {
    // see if we can add to the message
    lastDialAddedChar = didInputAlterMsg(dialSequence[0], dialSequence[1]);
  }
  return lastDialAddedChar;
}

std::string getSmsMessage() {
  return theMessage;
}

std::string addToMessage(char c)
{
  theMessage.push_back(c);
  return theMessage;
}

bool deleteChar() {
  if(!theMessage.empty()) {
    theMessage.pop_back();
    return true;
  } else {
    return false;
  }
}

void clearMessage()
{
  theMessage = "";
}

bool didInputAlterMsg(int charGroup, int charPosition)
{
  if (charGroup == 1 && charPosition == 1)
  {
    addToMessage(' ');
    return true;
  }
  if (charGroup == 1 && charPosition == 2)
  {
    addToMessage('.');
    return true;
  }
  if (charGroup == 1 && charPosition == 3)
  {
    addToMessage('!');
    return true;
  }
  if (charGroup == 1 && charPosition == 4)
  {
    addToMessage('?');
    return true;
  }
  if (charGroup == 1 && charPosition == 5)
  {
    addToMessage('-');
    return true;
  }

  // 2
  if (charGroup == 2 && charPosition == 1)
  {
    addToMessage('a');
    return true;
  }
  if (charGroup == 2 && charPosition == 2)
  {
    addToMessage('b');
    return true;
  }
  if (charGroup == 2 && charPosition == 3)
  {
    addToMessage('c');
    return true;
  }

  // 3
  if (charGroup == 3 && charPosition == 1)
  {
    addToMessage('d');
    return true;
  }
  if (charGroup == 3 && charPosition == 2)
  {
    addToMessage('e');
    return true;
  }
  if (charGroup == 3 && charPosition == 3)
  {
    addToMessage('f');
    return true;
  }

  // 4
  if (charGroup == 4 && charPosition == 1)
  {
    addToMessage('g');
    return true;
  }
  if (charGroup == 4 && charPosition == 2)
  {
    addToMessage('h');
    return true;
  }
  if (charGroup == 4 && charPosition == 3)
  {
    addToMessage('i');
    return true;
  }

  // 5
  if (charGroup == 5 && charPosition == 1)
  {
    addToMessage('j');
    return true;
  }
  if (charGroup == 5 && charPosition == 2)
  {
    addToMessage('k');
    return true;
  }
  if (charGroup == 5 && charPosition == 3)
  {
    addToMessage('l');
    return true;
  }

  // 6
  if (charGroup == 6 && charPosition == 1)
  {
    addToMessage('m');
    return true;
  }
  if (charGroup == 6 && charPosition == 2)
  {
    addToMessage('n');
    return true;
  }
  if (charGroup == 6 && charPosition == 3)
  {
    addToMessage('o');
    return true;
  }

  // 7
  if (charGroup == 7 && charPosition == 1)
  {
    addToMessage('p');
    return true;
  }
  if (charGroup == 7 && charPosition == 2)
  {
    addToMessage('q');
    return true;
  }
  if (charGroup == 7 && charPosition == 3)
  {
    addToMessage('r');
    return true;
  }
  if (charGroup == 7 && charPosition == 4)
  {
    addToMessage('s');
    return true;
  }

  // 8
  if (charGroup == 8 && charPosition == 1)
  {
    addToMessage('t');
    return true;
  }
  if (charGroup == 8 && charPosition == 2)
  {
    addToMessage('u');
    return true;
  }
  if (charGroup == 8 && charPosition == 3)
  {
    addToMessage('v');
    return true;
  }

  // 9
  if (charGroup == 9 && charPosition == 1)
  {
    addToMessage('w');
    return true;
  }
  if (charGroup == 9 && charPosition == 2)
  {
    addToMessage('x');
    return true;
  }
  if (charGroup == 9 && charPosition == 3)
  {
    addToMessage('y');
    return true;
  }
  if (charGroup == 9 && charPosition == 4)
  {
    addToMessage('z');
    return true;
  }

  if (charGroup == 10 && charPosition == 1)
  {
    deleteChar();
    return true;
  }
  if (charGroup == 10 && charPosition == 10)
  {
    clearMessage();
    return true;
  }

  return false;
}